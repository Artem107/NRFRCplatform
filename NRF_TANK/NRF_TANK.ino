#include "function.h"
#include "define.h"
#include "nrfLib.h"
#include "interrupt.h"

union packetRX {
  uint8_t buffer[32];
  struct messege {
    uint8_t peref1, peref2;
    int8_t motor1, motor2;
  } msg;
} packRX;

union packetTX {
  uint8_t buffer[32];
  struct messege {
    uint8_t stat1, stat2, stat3, peref1, peref2;
    uint16_t voltage;
    uint8_t ampl1, ampl2;
  } msg;
} packTX;

int main(void)
{
  Serial.begin(250000);
  ADC_init();
  initMove();
  initTimer();
  nrfInit();//Инициализация переферии для работы с nrf
  nrfConf();//Настройка модуля
  nrfSR;//Установка nrf в режим приема
  //nrfSAD(0xC715A8C3, 0);
  SCE;//Переход в режим приема/передачи
  sei();
  move(0, 0);
  _delay_ms(10);
  nrfAA(0b111111);
  nrfW(RX_PW_P0,32);
  nrfW(RX_PW_P1,32);
  nrfW(RX_PW_P2,32);
  nrfW(RX_PW_P3,32);
  nrfW(RX_PW_P4,32);
  nrfW(RX_PW_P5,32);
  RCSN;
  spiWR(FLUSH_TX);
  SCSN;
  while (1) {
    //Serial.println(nrfR(0x1D), 2);
    if (nrfAD != 0b111) {
      //Serial.println(ADC_result(7));
      packTX.msg.voltage = ADC_result(7);
      nrfRD(packRX.buffer, packTX.buffer, nrfAD);
      Serial.println(packRX.msg.motor1);
      move(packRX.msg.motor1, packRX.msg.motor2);
    }
    //_delay_ms(100);
  }
}


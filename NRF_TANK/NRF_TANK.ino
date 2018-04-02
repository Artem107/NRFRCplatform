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
  while (1) {
    if (nrfAD != 0b111) {
      nrfRD(packRX.buffer, packTX.buffer, nrfAD);
      move(packRX.msg.motor1, packRX.msg.motor2);
      Serial.println(packRX.msg.motor1);
    }
  }
}


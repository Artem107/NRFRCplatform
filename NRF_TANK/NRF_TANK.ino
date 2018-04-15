#include "function.h"
#include "define.h"
#include "nrfLib.h"
#include "interrupt.h"

packetRX packTX;
packetTX packRX;

uint32_t lastCycles;

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
  RCSN;
  spiWR(FLUSH_TX);
  SCSN;
  while (1) {
    if (nrfAD != 0b111) {
      packTX.msg.voltage = ADC_result(7);
      nrfRD(packRX.buffer, packTX.buffer, nrfAD);
      move(packRX.msg.motor1, packRX.msg.motor2);
      lastCycles = cycles;
    }
    if(cycles-lastCycles>20)move(0, 0);
    //_delay_ms(100);
  }
}


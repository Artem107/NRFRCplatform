#include "function.h"
#include "define.h"
#include "nrfLib.h"
#include "interrupt.h"

packetTX packTX;
packetRX packRX;

char buf[6], lastC;
uint8_t count;

int main(void)
{
  ADC_init();

  Serial.begin(500000);
  sei();
  nrfInit();//Инициализация переферии для работы с nrf
  nrfConf();//Настройка модуля
  nrfST;//Установка nrf в режим передачи
  initTimer();
  //nrfSAD(0xC715A8C3, 0);///Изменение адресса трубы
  //nrfSAD(0xC715A8C3, 6);

  while (1) {
    if (Serial.available()) {
      char c = Serial.read();
      if (count < 6)buf[count++] = c;
      if (c == 't' && lastC == 's')count = 0;
      if (c == 'o' && lastC == 'd') {
        packTX.msg.motor1 = buf[0];
        packTX.msg.motor2 = buf[1];
        nrfSD(packTX.buffer, packRX.buffer);
      }
      lastC = c;
      Serial.println(cycles);
    }
  }
}


//activ led 1 pin2
//Red pin3
//active led2 pin4
//Green pin5
//Blue pin6

#include "function.h"
#include "define.h"
#include "nrfLib.h"

union packetTX {
  uint8_t buffer[32];
  struct messege {
    uint8_t peref1, peref2;
    int8_t motor1, motor2;
  } msg;
} packTX;

union packetRX {
  uint8_t buffer[32];
  struct messege {
    uint8_t stat1, stat2, stat3, peref1, peref2;
    uint16_t voltage;
    uint8_t amplif1, amplif2;
  } msg;
} packRX;

int main(void)
{
  ADC_init();

  Serial.begin(250000);
  sei();
  nrfInit();//Инициализация переферии для работы с nrf
  nrfConf();//Настройка модуля
  nrfST;//Установка nrf в режим приема
  nrfSAD(0xC715A8C3, 0);///Изменение адресса трубы
  nrfSAD(0xC715A8C3, 6);
  Serial.println("Ready");

  while (1) {
    switch (key()) {
      case 1:
        packTX.msg.motor1 = -64;
        packTX.msg.motor2 = 64;
        nrfSD(packTX.buffer, packRX.buffer);
        break;
      case 2:
        packTX.msg.motor1 = 64;
        packTX.msg.motor2 = 64;
        nrfSD(packTX.buffer, packRX.buffer);
        break;
      case 3:
        packTX.msg.motor1 = -64;
        packTX.msg.motor2 = -64;
        nrfSD(packTX.buffer, packRX.buffer);
        break;
      case 4:
        packTX.msg.motor1 = 64;
        packTX.msg.motor2 = -64;
        nrfSD(packTX.buffer, packRX.buffer);
        break;
      default:
        packTX.msg.motor1 = 0;
        packTX.msg.motor2 = 0;
        nrfSD(packTX.buffer, packRX.buffer);
        break;
    }
    Serial.print(packTX.buffer[2]);
    Serial.print(" ");
    Serial.println(packTX.buffer[3]);
  }
}

uint8_t key() {
  uint16_t a = ADC_result(7);
  if (a < 70)return 1;
  if (a < 212)return 2;
  if (a < 398)return 3;
  if (a < 573)return 4;
  if (a < 810)return 5;
  return 0;
}

#include "function.h"
#include "define.h"
#include "nrfLib.h"

uint8_t key;
uint8_t rxB[32];
uint8_t txB[32];

int main(void)
{
  ADC_init();

  Serial.begin(250000);
  sei();
  nrfInit();//Инициализация переферии для работы с nrf
  nrfConf();//Настройка модуля
  nrfST;//Установка nrf в режим приема
  nrfSAD(0xC715A8C3, 0);
  nrfSAD(0xC715A8C3, 6);
  Serial.println("Ready");

  memset(txB, 0, 32);

  while (1) {
    if (nrfSD(txB, rxB)) {
      for (int i = 0; i != 32; i++) {
        Serial.print(rxB[i]);
        Serial.print(" ");
      }
      Serial.println("OK");
    }
    else Serial.println("Err");
    //_delay_us(1000);
    txB[0] = key1();
  }
}

uint8_t key1() {
  uint16_t a = ADC_result(7);
  if (a < 70)return 1;
  if (a < 212)return 2;
  if (a < 398)return 3;
  if (a < 573)return 4;
  if (a < 810)return 5;
  return 0;
}

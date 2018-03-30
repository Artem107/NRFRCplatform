#include "function.h"
#include "define.h"
#include "nrfLib.h"

uint8_t a;
uint8_t rxB[32];
uint8_t txB[32];

int main(void)
{
  Serial.begin(250000);
  sei();
  nrfInit();//Инициализация переферии для работы с nrf
  nrfConf();//Настройка модуля
  nrfSR;//Установка nrf в режим приема
  nrfSAD(0xC71918C3, 0);
  nrfSAD(0xC715A8C3, 1);
  SCE;//Переход в режим приема/передачи
  memset(txB, 10, 32);

  while (1) {
    if (nrfAD != 0b111) {
      nrfRD(rxB, txB, nrfAD);
      /*      for (int i = 0; i != 32; i++) {
                    Serial.print(rxB[i]);
                    Serial.print(" ");
                  }
                  Serial.println("OK");*/
    }
    if (rxB[0] != a) {
      Serial.println(rxB[0]);
      a = rxB[0];
    }
    txB[2] = rxB[0];
  }
}


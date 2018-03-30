#include "function.h"
#include "define.h"

static void ADC_init() {
  ADMUX = 0b01000000;
  ADCSRA = 0b10000111;
}

uint16_t ADC_result(uint8_t adc_input) {
  ADMUX = (adc_input & 0x0F) | (ADMUX & 0xF0);
  ADCSRA |= 0x40;
  while ((ADCSRA & 0x10) == 0);
  return ADCW;
}

void spi_init() {
  resR(DDRB, 0b010000); //Конфигурируем пины
  setR(DDRB, 0b101000);
  SPCR = 0b01010001;//Включаем и настраиваем spi
  SPSR = 0b1;//Удвоенная скорость
}

uint8_t spiWR(uint8_t data) {
  SPDR = data;//Начинаем передачу данных
  while ((SPSR & (1 << SPIF)) == 0);//Ожидание окончание передачи
  return SPDR;//Возращаем принятые данные и сбрасываем флаг передачи
}

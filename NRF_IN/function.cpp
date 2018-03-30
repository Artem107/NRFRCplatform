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

void initMove() {
  DDRD |= 0xF0;
  TCCR0A = 0b10100011;
  TCCR0B = 0b001;
}

void initTimer() {
  TCCR1B = 0b001;
  TIMSK1 = 1;
}

uint32_t getCycles(){
  return TCNT1+(cycles<<16);
}

void move(int8_t m1, int8_t m2) {
  OCR0B = m1 << 1;
  OCR0A = m2 << 1;
  PORTD = (PORTD & 0x6F) | ((m1 & 0x80) >> 3) | (m2 & 0x80);
}

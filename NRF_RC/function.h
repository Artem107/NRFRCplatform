#ifndef FUNCTION
#define FUNCTION

#include <stdint.h>
#include <avr/io.h>

extern uint32_t cycles;

void ADC_init();
uint16_t ADC_result(uint8_t);
void spi_init();
uint8_t spiWR(uint8_t);
void initTimer();
uint32_t getCycles();

#endif

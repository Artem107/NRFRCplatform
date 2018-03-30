#ifndef INTERRUPT
#define INTERRUPT

uint32_t cycles;

ISR(TIMER1_OVF_vect) {
  cycles++;
}

#endif

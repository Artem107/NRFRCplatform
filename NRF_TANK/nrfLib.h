#ifndef NRFLIB
#define NRFLIB

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "define.h"
#include "src/nrfDEFINE.h"
#include "function.h"

#define nrfCH(a) nrfW(RF_CH,(a))

#define nrfST nrfW(CONFIG, 0b1110)
#define nrfSR nrfW(CONFIG, 0b1111)
#define nrfEP(a) nrfW(EN_RXADDR, a)
#define nrfAW(a) nrfW(SETUP_AW, a)
#define nrfRETR(a) nrfW(SETUP_RETR, a)
#define nrfPDP(a,b) nrfW(RX_PW_P0 + b, a)
#define nrfFR(a) nrfW(FEATURE, a)
#define nrfDPL(a) nrfW(DYNPD, a)
#define nrfRF(a) nrfW(RF_SETUP, a)
#define nrfAD ((nrfR() >> 1 ) | 0x7)
#define nrfAA(a) nrfW(EN_AA, a)

#define SCE  setB(2)
#define SCSN setB(1)
#define RCE  resB(2)
#define RCSN resB(1)

extern uint8_t CH;

union packetTX {
  uint8_t buffer[32];
  struct messege {
    uint8_t peref1, peref2;
    int8_t motor1, motor2;
  } msg;
};

union packetRX {
  uint8_t buffer[32];
  struct messege {
    uint8_t stat1, stat2, stat3, peref1, peref2;
    uint16_t voltage;
    uint8_t ampl1, ampl2;
  } msg;
};

void nrfInit();
void nrfConf();
uint8_t nrfR(uint8_t);
uint8_t nrfR();
void nrfW(uint8_t, uint8_t);
void nrfSAD(uint32_t, uint8_t);
uint8_t nrfSD(uint8_t*);
uint8_t nrfSD(uint8_t*, uint8_t*);
void nrfRD(uint8_t*);
void nrfRD(uint8_t*, uint8_t*, uint8_t);
uint8_t nrfNP();

#endif

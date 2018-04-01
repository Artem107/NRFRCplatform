#ifndef DEFINE
#define DEFINE

#define outB(a) DDRB|=(1<<(a))
#define outC(a) DDRC|=(1<<(a))
#define outD(a) DDRD|=(1<<(a))

#define inB(a) DDRB&=~(1<<(a))
#define inC(a) DDRC&=~(1<<(a))
#define inD(a) DDRD&=~(1<<(a))

#define setB(a) PORTB|=(1<<(a))
#define setC(a) PORTC|=(1<<(a))
#define setD(a) PORTD|=(1<<(a))

#define resB(a) PORTB&=~(1<<(a))
#define resC(a) PORTC&=~(1<<(a))
#define resD(a) PORTD&=~(1<<(a))

#define getB(a) (PINB>>(a)&1)
#define getC(a) (PINC>>(a)&1)
#define getD(a) (PIND>>(a)&1)

#define setR(a,b) (a)|=(b)
#define resR(a,b) (a)&=~(b)

#define setRB(a,b) (a)|=(1<<b)
#define resRB(a,b) (a)&=~(1<<b)

#define getR(a,b) ((a)>>(b)&1)

#endif

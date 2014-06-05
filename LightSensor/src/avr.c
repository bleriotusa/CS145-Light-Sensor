#include "avr.h"



void
ini_avr(void)
{
	WDTCR = 15;
	
	SET_BIT(DDRA, 0);
	
	CLR_BIT(DDRC, 4);
	CLR_BIT(DDRC, 5);
	CLR_BIT(DDRC, 6);
	CLR_BIT(DDRC, 7);
	SET_BIT(DDRC, 0);
	SET_BIT(DDRC, 1);
	SET_BIT(DDRC, 2);
	SET_BIT(DDRC, 3);
}


void
wait_avr(unsigned short msec)
{
  TCCR0 = 3;
  while (msec--) {
    TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * .001);
    SET_BIT(TIFR, TOV0);
    WDR();
    while (!GET_BIT(TIFR, TOV0));
  }
  TCCR0 = 0;
}

void setupInterrupt(unsigned short msec)
{
//	cli();
	
// 	TCCR0 = 5;
// 	OCR0 = 255;//(msec/1000)/0.000128;
// 	SET_BIT(TIMSK, OCIE0);
// 	SET_BIT(TCCR0, WGM01); 
// 	
// 	sei();
	
	cli();
	
	TCCR1B = 13;
	OCR1A = msec;
	SET_BIT(TIMSK, OCIE1A);
	
	sei();
	
}

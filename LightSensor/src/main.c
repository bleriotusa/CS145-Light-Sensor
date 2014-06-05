/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <avr.h>
#include <lcd.h>
#include <stdio.h>
#include <interrupt.h>

volatile unsigned char buff[10];
volatile unsigned short current, max, ave, total, counter = 0;
volatile unsigned short min = 1023;


unsigned short sample(void)
{
	SET_BIT(ADCSRA, ADSC); // start conversion
	while(!GET_BIT(ADCSRA, ADIF)); // wait til ADIF is 1 to signal conversion complete
	SET_BIT(ADCSRA, ADIF); // reset flag
	return ADC; // read result
}

void display(unsigned short current, unsigned short max, unsigned short min, unsigned short ave)
{
	//current 
	clr_lcd();
	

	char buffer[7];
	
	pos_lcd(0,0);
	sprintf(buffer, "CUR:%i", current);
	put_str_lcd(buffer);
}
ISR(TIMER1_COMPA_vect)
{
 	current = sample();
 	
 	if(current > max)
 	{
 		max = current;
 	}
 	
 	else if(current <= min)
 	{
 		min = current;
 	}
	display(current, max, min, ave);
	if(current < max-200)
		changeLights();
		
	wait_avr(100);
	SET_BIT(TIFR, 4);
}

void changeLights(void)
{
	counter++;
	if(counter % 2 == 1)
	{
		CLR_BIT(PORTC, 0);
		SET_BIT(PORTC, 1);

		wait_avr(2000);
		CLR_BIT(PORTC, 1);
		SET_BIT(PORTC, 2);
		
		CLR_BIT(PORTC, 5);
		SET_BIT(PORTC, 3);
	}
	else if(counter % 2 == 0)
	{
		CLR_BIT(PORTC, 3);
		SET_BIT(PORTC, 4);
		
		wait_avr(2000);
		CLR_BIT(PORTC, 4);
		SET_BIT(PORTC, 5);
		
		CLR_BIT(PORTC, 2);
		SET_BIT(PORTC, 0);
	}
}

int main (void)
{

	ini_lcd();
	ini_avr();
	//ini_keyPad();
	put_str_lcd("Welcome");
	
	SET_BIT(DDRC, 0);
	SET_BIT(DDRC, 1);
	SET_BIT(DDRC, 2);
	SET_BIT(DDRC, 3);
	SET_BIT(DDRC, 4);
	SET_BIT(DDRC, 5);
	SET_BIT(PORTC, 0);
	CLR_BIT(PORTC, 1);
	CLR_BIT(PORTC, 2);
	CLR_BIT(PORTC, 3);
	CLR_BIT(PORTC, 4);
	SET_BIT(PORTC, 5);


	ADCSRA = 0; // clear ADCSRA
	ADMUX = 65; // 64 for bit 6 + 1 for bit 1

	SET_BIT(ADCSRA, ADEN); // set ADC on
	setupInterrupt(1000);
	
	while(1)
	{
		wait_avr(20000);
		//changeLights();
	}
}

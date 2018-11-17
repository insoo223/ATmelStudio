/*
 * blinkLED_PB0.c
 *
 * Created: 2017-07-03 오후 5:34:54
 * Author : insoo
 */ 

#define F_CPU 1000000UL /*if not, there's warning. The clock is set by IDE, i.e. ATmel Studio 7*/

#include <avr/io.h>
#include <util/delay.h>

//function prototypes
void blinkLED_PB0();

//-------------------------------------------
int main(void)
{
	DDRB = _BV(PB0); // make PB0 (pin5) an output 
	OSCCAL -= 28; //calibrate internal OSC as of July 4, 2017

    for (;;) {
		blinkLED_PB0();
    }	

	return 0; // never reach here
}//main

//-------------------------------------------
void blinkLED_PB0()
{
	PORTB ^= 1 << PB0; // toggle the LED
	_delay_ms(500); //using oscilloscope, measured correct as of July 4, 2017
	/* max is 262.14 ms / F_CPU in MHz */
}//blinkLED_PB0

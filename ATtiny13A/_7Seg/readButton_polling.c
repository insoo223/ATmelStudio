/*
 * readButton_polling.c
 *
 * Created: 2017-07-03 오후 5:34:54
 * Author : insoo
 */ 

#define F_CPU 1000000UL /*if not, there's warning. The clock is set by IDE, i.e. ATmel Studio 7*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//function prototypes
void chkButton();

//-------------------------------------------
// Interrupt Service Routines

//-------------------------------------------
int main(void)
{
	DDRB |= _BV(PB0); // make PB0 (pin5) an output 
	DDRB &= ~_BV(PB1); // make PB1 (pin6) an input 

	OSCCAL -= 28; //calibrate internal OSC as of morning July 4, 2017

    for (;;) {
		chkButton();
    }	

	return 0; // never reach here
}//main

void chkButton()
{
	uint8_t val;
	val = PINB & _BV(PB1);//read PB1
	if (val == 0)	//if pressed
		PORTB |= _BV(PB0);	//turn on LED on PB0
	else //if released
		PORTB &= ~_BV(PB0); //turn off LED on PB0
}//chkButton
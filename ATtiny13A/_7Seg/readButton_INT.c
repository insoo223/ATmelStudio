/*
 * readButton_INT.c
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

//-------------------------------------------
// Interrupt Service Routines
//-----------------------------------
ISR(PCINT0_vect)
{
    // Toggle PB0 output state
    PORTB ^= 1<<PB0;

}//ISR(PCINT0_vect)


//-------------------------------------------
int main(void)
{
	//pin IO setting
	DDRB |= _BV(PB0); // make PB0 (pin5) an output 
	DDRB &= ~_BV(PB1); // make PB1 (pin6) an input 

	//internal oscilator tuning
	OSCCAL -= 28; //calibrate internal OSC as of morning July 4, 2017

    //PC interrupt setting
    // enable PC(Pin Change) interrupt
    GIMSK |= _BV(PCIE);  //Enable PC interrupt
    // Enable pin change interrupt for PB3
    //PCMSK |= _BV(PCINT3);
    PCMSK |= _BV(PCINT1);

    for (;;) {
    }	

	return 0; // never reach here
}//main


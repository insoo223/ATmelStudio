/*
 * blinkPB0_ISR.c
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
ISR(WDT_vect) {
	// Toggle PB0 output state
	PORTB ^= 1<<PB0;
}

//-------------------------------------------
int main(void)
{
	DDRB = _BV(PB0); // make PB0 (pin5) an output 
	OSCCAL -= 28; //calibrate internal OSC as of July 4, 2017

    // temporarily prescale timer to 4s so we can measure current
    WDTCR |= (1<<WDP3); // (1<<WDP2) | (1<<WDP0);

    // Enable watchdog timer interrupts
    WDTCR |= (1<<WDTIE);

    sei(); // Enable global interrupts

    // Use the Power Down sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    for (;;) {
	    sleep_mode();   // go to sleep and wait for interrupt...
    }	

	return 0; // never reach here
}


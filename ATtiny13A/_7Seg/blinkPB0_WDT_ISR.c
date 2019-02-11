/*
 * blinkPB0_WDT_ISR.c
 *
 * Created: 2017-07-03 오후 5:34:54
 * Author : insoo
 * Desc
	Pwr usage in sleep: 60 uA
			  in working w/LED on: 2000 uA
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
	OSCCAL -= 28; //calibrate internal OSC as of morning July 4, 2017
	//OSCCAL += 20; //calibrate internal OSC as of pm1:30 July 4, 2017

    // temporarily prescale timer to 4s so we can measure current
    //WDTCR |= (1<<WDP3); // 4s 
	//WDTCR |= (1<<WDP2); // 0.25s -> 0.24ms
	WDTCR |= (1<<WDP2) | (1<<WDP0); // 0.5s -> 0.48s
	//WDTCR |= (1<<WDP1); // 64ms -> 60ms
	//WDTCR |= (1<<WDP0); // 32ms -> 30ms

    // Enable watchdog timer interrupts
    WDTCR |= (1<<WDTIE);

    sei(); // Enable global interrupts

    // Use the Power Down sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    for (;;) {
	    sleep_mode();   // go to sleep and wait for interrupt...
		//blinkLED_PB0();
    }	

	return 0; // never reach here
}


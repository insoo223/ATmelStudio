/*
 * readButton_PinChangeINT.c
 *
 * Created: 2017-07-03 오후 5:34:54
 * Author : insoo
 * Desc: Check button input on PB1 or PCINT1 and change LED on PB0
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
volatile uint8_t done = 0;
ISR(PCINT0_vect)
{
	/* Pin change event occurs up & falling edge
	   so that even or odd number event can trigger
	   only once per single press of the button */
	if (++done % 2 == 0)
	{
		// Toggle PB0 output state
		PORTB ^= 1<<PB0;
	}
	_delay_ms(200); //debounce 

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
    // Enable pin change interrupt for PB1
    PCMSK |= _BV(PCINT1);
    // enable PC(Pin Change) interrupt
    GIMSK |= _BV(PCIE);  //Enable PC interrupt
	sei();

	// Use the Power Down sleep mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//set_sleep_mode(SLEEP_MODE_IDLE);

    for (;;) {
        // go to sleep and wait for interrupt...
        sleep_mode();
    }	

	return 0; // never reach here
}//main


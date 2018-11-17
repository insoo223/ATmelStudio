#define F_CPU 1200000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>


ISR( INT0_vect) {
	PORTB ^= _BV(PB3); // Toggle PB1
}

int main(void) {
	DDRB &= ~_BV(DDB0); // PB0 as input
	DDRB |= _BV(PB3); //PB3 as output
	sei();

	while (1) 
	{
		PORTB ^= _BV(PB3);
		_delay_ms(1000);

	}

	return 0; // never reached
}

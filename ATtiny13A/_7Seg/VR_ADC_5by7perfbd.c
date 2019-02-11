/*
 * VR_ADC_5by7perfbd.c
 *
 * Created: 2017-07-05 오후 3:39:01
 *  Author: insoo
 */ 

#define F_CPU 9600000 /*if not, there's warning. The clock is set by IDE, i.e. ATmel Studio 7*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define LED_PIN		PB0  // for 5*7 perf b'd
#define BUTTON_PIN	PB1  // for 5*7 perf b'd
#define VR_PIN		PB3  // for 5*7 perf b'd

#define DURATION_BLINKLED 200 // [ms]

//Global variables

//function prototypes
void blinkLED(uint8_t );
void tuningCLK();
void initIO();
void initADC();
void setADC_ClockSpeed();
int adc_read ();
void setPWM_Clock();
void setPWM_Mode();
void pwm_write (int );

//-------------------------------------------
//Interrupt Service Routines
//-----------------------------------


//-------------------------------------------
int main(void)
{
	int resultADC; 

	tuningCLK();
	initIO();
	initADC();
	setADC_ClockSpeed();

	setPWM_Clock();
	setPWM_Mode();

    for (;;) 
	{
		resultADC = adc_read ();
		//blinkLED(resultADC/10);
		//_delay_ms(500);
		pwm_write(resultADC);

    }//for

	return 0; // never reach here
}//main

//-----------------------------------
void blinkLED(uint8_t num)
{
	uint8_t i;
	for (i=0; i<(2*num); i++)
	{
		//digitalWrite(ledPin, HIGH);
		PORTB ^= _BV(LED_PIN);
		_delay_ms(DURATION_BLINKLED);
	}
	//BLINK_NOTICED = 1;
	PORTB &= ~_BV(LED_PIN);
	_delay_ms(DURATION_BLINKLED);
}//blinkLED

void tuningCLK()
{
	//internal oscillator tuning
	OSCCAL -= 28; //calibrate internal OSC as of morning July 4, 2017

	//CLKPR = _BV(CLKPCE) | _BV(CLKPS3);
	//CLKPR = 0x00;
	//CLKPR = 0x88; // DIV256 system clock
	//CLKPR = 0x00; // these 2 CLKPR instructions have to be run together in order
}//tuningCLK

void initIO()
{
	//pin IO setting
	DDRB |= _BV(LED_PIN); // make LED_PIN (PB0, pin5) an output
	DDRB &= ~_BV(BUTTON_PIN); // make BUTTON_PIN (PB1, pin6) an input
	DDRB &= ~_BV(VR_PIN); // make BUTTON_PIN (PB1, pin6) an input
}//initIO

void initADC()
{
	//ADC setting
	//Enable ADC pin input from VR_PIN (PB3 or ADC3)
	ADMUX |= _BV(MUX1) | _BV(MUX0);
	//ADC Left Adjust Result
	ADMUX |= _BV(ADLAR);  

}//initADC

void setADC_ClockSpeed()
{
	//Set the prescaler to clock/128 & enable ADC
	ADCSRA |= _BV(ADPS1) | _BV(ADPS0);

	// enable ADC
	ADCSRA |= _BV(ADEN);

}//setADC_ClockSpeed

int adc_read (void)
{
	// Start the conversion
	ADCSRA |= (1 << ADSC);

	// Wait for it to finish - blocking
	while (ADCSRA & (1 << ADSC));

	return ADCH;
}//adc_read

void setPWM_Clock()
{
	// Set Timer 0 prescaler to clock/8.
	// At 9.6 MHz this is 1.2 MHz.
	TCCR0B |= (1 << CS01) | (1 << CS00);

}//setPWM_Clock

void setPWM_Mode()
{
	// Set to 'Fast PWM' mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);

	// Clear OC0B output on compare match, upwards counting.
	//TCCR0A |= (1 << COM0B1); //for PB1
	TCCR0A |= (1 << COM0A1); //for PB0
}//setPWM_Mode

void pwm_write (int val)
{
	//OCR0B = val; //for PB1
	OCR0A = val; //for PB0
}
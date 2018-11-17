/*
 * AVR_DHT11_PCintr_PwrSave.c
 *
 * Created: 2017-07-08 오후 2:12:44
 * Author : insoo
 */ 

#define F_CPU 1000000 /*if not, there's warning. The clock is set by IDE, i.e. ATmel Studio 7*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//#include "../Library/DHT11/tinudht/tinudht.h" //DHT11 library hader
#include "C:\Users\insoo\Box Sync\BoxElec\DIY Electronics\Arduino\AVR C\Win_Atmel Studio_Codes\7.0\ATtiny13A\Library\DHT11\tinudht\tinudht.h" //DHT11 library hader

#define LED_PIN		PB0  // for 5*7 perf b'd
#define BUTTON_PIN	PB1  // for 5*7 perf b'd
#define DHTVCC_PIN	PB2  // for 5*7 perf b'd
#define DHTSIG_PIN PB3  // for 5*7 perf b'd


#define DURATION_BLINKLED 200 // [ms]
#define BUTTONCNT_TIMELIMIT 2 // [s]

//Global variables

//function prototypes
void blinkLED(uint8_t );
void tuningCLK();
void initIO();
void chkTempHumid();

//-------------------------------------------
//Interrupt Service Routines
//-----------------------------------
void initINTR();
void initINTR_PinChange();

//-------------------------------------------
//Interrupt Service Routines
//-----------------------------------

volatile uint8_t done = 0;

ISR(PCINT0_vect)
{
	if (++done % 2 == 0)
	{
		//PORTB ^= _BV(LED_PIN);
		chkTempHumid();
	}
	_delay_ms(200); //debounce

}//ISR(PCINT0_vect)

//-------------------------------------------
int main(void)
{

	tuningCLK();
	initIO();

	initINTR_PinChange();

	sei();

	// Use the Power Down sleep mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//set_sleep_mode(SLEEP_MODE_ADC);
	//set_sleep_mode(SLEEP_MODE_IDLE);

	sleep_enable();

	for (;;)
	{
		// go to sleep and wait for interrupt...
		//sleep_mode();
		sleep_cpu();
	}//for

	return 0; // never reach here
}//main

void chkTempHumid()
{
	TinuDHT tinudht;
	uint16_t temp, temp10, temp1;
	uint16_t humid, humid10, humid1;

	PORTB |= _BV(DHTVCC_PIN);// turn on DHT11
	_delay_ms(1000);
	tinudht_read(&tinudht, DHTSIG_PIN);

	temp = tinudht.temperature;
	temp10 = temp/10;
	temp1 = temp %10;
	blinkLED(temp10);
	_delay_ms(500);
	blinkLED(temp1);
	_delay_ms(500);

	humid = tinudht.humidity;
	humid10 = humid/10;
	humid1 = humid %10;
	blinkLED(humid10);
	_delay_ms(500);
	blinkLED(humid1);
	_delay_ms(1000);

	PORTB &= ~_BV(DHTVCC_PIN);// turn off DHT11
	//DDRB |= _BV(DHTSIG_PIN);// make DHTSIG_PIN as output
	//DDRB &= ~_BV(DHTSIG_PIN);// make DHTSIG_PIN as input
	//PORTB &= ~_BV(DHTVCC_PIN);// turn off DHT11


}
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
	//PORTB &= ~_BV(LED_PIN);
	//_delay_ms(DURATION_BLINKLED);
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
	DDRB |= _BV(DHTVCC_PIN); // make LED_PIN (PB2, pin7) an output
	DDRB &= ~_BV(DHTSIG_PIN); // make BUTTON_PIN (PB3, pin6) an input
}//initIO

void initINTR_PinChange()
{
	//PC interrupt setting
	// Enable pin change interrupt for BUTTON_PIN
	PCMSK |= _BV(PCINT1);
	// enable PC(Pin Change) interrupt
	GIMSK |= _BV(PCIE);  //Enable PC interrupt

}//initINTR_PinChange



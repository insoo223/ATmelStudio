/*
 * DHT11_OneLEDdisp.c
 *
 * Created: 2017-07-06 오전 7:57:23
 *  Author: insoo
 */ 

/* F_CPU should be 1Mhz for the DHT11 library working properly */
#define F_CPU 1000000 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

//#include "../Library/DHT11/tinudht/tinudht.h" //DHT11 library hader
#include "C:\Users\insoo\Box Sync\BoxElec\DIY Electronics\Arduino\AVR C\Win_Atmel Studio_Codes\7.0\ATtiny13A\Library\DHT11\tinudht\tinudht.h" //DHT11 library hader

#define LED_PIN		PB0  // for 5*7 perf b'd
#define BUTTON_PIN	PB1  // for 5*7 perf b'd
#define TINUDHT_PIN PB3  // for 5*7 perf b'd


#define DURATION_BLINKLED 200 // [ms]

//Global variables

//function prototypes
void blinkLED(uint8_t );
void tuningCLK();
void initIO();

//-------------------------------------------
//Interrupt Service Routines
//-----------------------------------


//-------------------------------------------
int main(void)
{
	uint16_t temp, temp10, temp1; 
	uint16_t humid, humid10, humid1; 
	TinuDHT tinudht;

	tuningCLK();
	initIO();


    for (;;) 
	{
		tinudht_read(&tinudht, TINUDHT_PIN);
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
		_delay_ms(500);
		//blinkLED(humid/10);
		//_delay_ms(1000);

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
	DDRB &= ~_BV(TINUDHT_PIN); // make BUTTON_PIN (PB1, pin6) an input
}//initIO


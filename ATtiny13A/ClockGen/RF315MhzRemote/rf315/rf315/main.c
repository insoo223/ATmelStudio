/**************************************************************
 Target MCU & clock speed: ATtiny13A @ 1.2Mhz internal
 Name: main.c
	Project folder: 
	C:\Users\Insoo\Documents\GitHub\ATmelStudio\ATtiny13A\ClockGen\RF315MhzRemote\rf315\rf315
	rf315
 C modules of this project:
    Nothing
 Custom Headers:
    Nothing
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : Aug 22, 2020 (On Atmel Studio 7)
 Updated : 
		Oct 17, 2018 (On Atmel Studio 7)

 Description: 
	Turn on and off AC power appliance like summer fan using RF315Mhz remote.
	Press RF remote button B to power On and D to Off

 HEX size[Byte]: 238 out of 1024

How to upload to the target MCU
<For Windows Atmel Studio>
Select Tool - USBtiny 
(USBtiny menu should be configured in the external tool menu)

<For CMD window or DOS prompt>
cd "C:\Users\Insoo\Documents\GitHub\ATmelStudio\ATtiny13A\ClockGen\RF315MhzRemote\rf315\rf315\Debug"
avrdude -c usbtiny -P usb -p attiny13 -U flash:w:rf315.hex:i

 ****************************************************************/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define halfSec 75

#define latchPort PORTB
#define latchPin  PB0 //latch
#define clockPort PORTB
#define clockPin  PB1 //clock
#define dataPort  PORTB
#define dataPin   PB2 //data

#define relayPin  PB3 //relay

#define latchLow()  latchPort&=~_BV(latchPin)
#define latchHi() latchPort|=_BV(latchPin)
#define clockLow()  clockPort&=~_BV(clockPin)
#define clockHi() clockPort|=_BV(clockPin)

//Define functions
//======================
void ioinit(void);
void readRF(char*);
//======================

ISR(PCINT0_vect)
{
	char val;

	readRF(&val);

	//if (val & 0x08) // always ON
	//if (val & 0x04) // always ON
	//if (val & 0x10) // Button D
	//if (val & 0x40) // always ON
	//if (val & 0x80) // always OFF
	if (val & 0x20) // Button B, On
	PORTB |= _BV(PB3);

	if (val & 0x10) // Button D, Off
	PORTB &= ~_BV(PB3);
}//ISR(PCINT0_vect)

int main (void)
{
	ioinit(); //Setup IO pins and defaults

	// enable PC(Pin Change) interrupt
	GIMSK |= _BV(PCIE);  //Enable PC interrupt

	// Enable pin change interrupt for PBn
	//VT of RF315 receiver chip is connected to PB4 of ATtiny13A
	PCMSK |= _BV(PCINT4);

	// enable global interrupt
	sei();

	// Use the Power Down sleep mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	while(1) {
		sleep_mode();   // go to sleep and wait for interrupt...
	}
}//main


void ioinit (void)
{
	DDRB  = 0x00; //1 = output, 0 = input
	DDRB |= _BV(latchPin); //output
	DDRB |= _BV(clockPin); //output
	DDRB &= ~_BV(dataPin); //input
	DDRB |= _BV(relayPin); //output

	PORTB = 0x00;
}//ioinit


void readRF(char* val)
{
	int i;
	char inputData=0;

	clockLow();
	latchLow();
	for (i=7;i>=0;i--)
	{
		inputData |= (PINB & _BV(dataPin)) << i;
		clockHi();
		clockLow();
	}
	latchHi();

	*val = inputData;

}//readRF


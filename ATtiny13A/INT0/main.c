/**************************************************************
 Target MCU & clock speed: ATtiny13A @ 1.2Mhz internal
 Name: main.c
	Project folder: ISR-Bathroom LED-23189812
 C modules of this project:
    Nothing
 Custom Headers:
    Nothing
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : Oct 11, 2018
 Updated : Oct 11, 2018 (On Atmel Studio 7)

 Description: 
	Interrupt programming for ATtiny13A

	INT0 is from PB1 and NOT from PB0!

 HEX size[Byte]: 104 out of 1024

How to upload to the target MCU
<For Windows Atmel Studio>
Select Tool - USBtiny 
(USBtiny menu should be configured in the external tool menu)

<For CMD window or DOS prompt>
cd "C:\Users\insoo\Documents\GitHub\ATmelStudio\ATtiny13A\INT0\Debug"
avrdude -c usbtiny -P usb -p attiny13 -U flash:w:INT0.hex:i

 Ref: 
*****************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// ATtiny13A port usage
#define SW PB0
#define LED PB1 //PB3
#ifndef F_CPU 
	#def F_CPU 1000000
#endif 

//-------- function prototypes --------
void systemInit(void);
void toggleLED(void);

//-------- interrupt service routines --------
ISR(INT0_vect)
{
	PORTB ^= _BV(LED);
}//ISR(WDT_vect)


int main(void)
{
    systemInit();

	// Use the Power Down sleep mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	
	// ADC noise reduction sleep mode
	//set_sleep_mode(SLEEP_MODE_ADC);

	while (1) 
	{
		// go to sleep and wait for interrupt...
		// 33 uA as of Sep 27, 2018 when sleep
		//sleep_mode();
		
		toggleLED();
		_delay_ms(200);

	}//while (1) 
}//main


void toggleLED(void)
{
	PORTB ^= (1<<LED);	
}//toggleLED0

void systemInit(void)
{
	// I/O
	DDRB |= _BV(LED); //output for LED
	DDRB &= ~_BV(SW); //input for SW

	// INT0 setup
	GIMSK |= _BV(INT0); // enable INT0 interrupt
	// Trigger INT0 on falling edge
	// but, CANNOT wake up from Power Down sleep
	MCUCR |= _BV(ISC01);	
	MCUCR &= _BV(ISC00);	
	// Trigger INT0 on low level, can wake up from Power Down sleep

	sei();          // enable all interrupts
}//systemInit


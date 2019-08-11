/**************************************************************
 Target MCU & clock speed: ATtiny13A @ 1.2Mhz internal
 Name    : main.c
 C modules of this project, ISR:
main.c
Custom Headers:
    Nothing
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : Sep 26, 2018
 Updated : Sep 27, 2018 (On Atmel Studio 7)

 Description: 
	ATtiny13A controls power based on PIR motion sensor input. 
	PIR sensor triggers PCINT1

 HEX size[Byte]: 176 out of 1024

How to upload to the target MCU
<For Windows Atmel Studio>
Slect Tool ? USBtiny (USBtiny memu should be configured in the external tool memu)

<For CMD window or DOS prompt>
cd " C:\Users\insoo\Documents\GitHub\ATmelStudio\ATtiny13A\ClockGen\ISR\Debug "
avrdude -c usbtiny -P usb -p attiny13 -U flash:w:ISR.hex:i

 Ref:PIR time duration & sensitivity control
 http://qqtrading.com.my/pir-motion-sensor-module-hc-sr501
*****************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define LED0 PB0
#define LED1 PB1
#define CDS5 PB2
#define PIR  PB3

uint8_t i;
ISR(PCINT0_vect)
{
	//GIMSK &= ~(1<<PCIE); //disable interrupt
	PORTB ^= (1<<PB0);
	for (i=0; i<60; i++)
		_delay_ms(1000);
	//GIMSK |= (1<<PCIE); //enable interrupt
	/*
	PORTB |= (1<<PB0);
	_delay_ms(300);
	PORTB &= ~(1<<PB0);
	_delay_ms(300);
	*/
}

void SystemInit(void)
{
	MCUCR &= ~(1<<ISC01) | ~(1<<ISC00);	// Trigger INT0 on rising edge
	PCMSK |= (1<<PCINT1);   // pin change mask: listen to portb, pin PB3
	GIMSK |= (1<<PCIE); // enable PCINT interrupt
	sei();          // enable all interrupts
}

int main(void)
{
	DDRB |= (1<<PB0);
	DDRB  &= ~(1<<PB1);

	
	SystemInit();

	// Use the Power Down sleep mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	while (1)
	{
		// go to sleep and wait for interrupt... 
		// 33 uA as of Sep 27, 2018 when sleep
		sleep_mode();   
						
	}
}
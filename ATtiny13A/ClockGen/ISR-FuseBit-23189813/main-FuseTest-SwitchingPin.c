/**************************************************************
 Target MCU & clock speed: ATtiny13A 
	@ 1.2Mhz internal (varied by fuse bit setting in avrdude)
 Name: main.c
	Project folder: ISR-FuseBit-23189813
 C modules of this project:
    Nothing
 Custom Headers:
    Nothing
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : Oct 02, 2018
 Updated : Oct 02, 2018 (On Atmel Studio 7)

 Description: 
	Fuse bit of AVR MCU is doing due job to manager power consumption
		by controlling internal clock speed. Here i try to experiment
		by changing fuse bit at the command line of avrdude,
		in accordance with datasheet p104

	ATtiny13A controls LED blink speed, and its blinking frequency will
		be different even with same _delay_ms(D).

 HEX size[Byte]:  out of 1024

How to upload to the target MCU
<For Windows Atmel Studio>
Select Tool - USBtiny 
(USBtiny menu should be configured in the external tool menu)

<For CMD window or DOS prompt>
cd " C:\Users\insoo\Documents\GitHub\ATmelStudio\ATtiny13A\ClockGen\ISR-FuseBit-23189813\Debug "
avrdude -c usbtiny -P usb -p attiny13 -U lfuse:w:0x6B:m flash:w:"ISR-FuseBit.hex":i

** To run 128Kbps OSC
avrdude -c usbtiny -P usb -p attiny13 -U lfuse:w:0x6B:m flash:w:"ISR-FuseBit.hex":i

** After buring the fuse as Internal 128Kbps OSC, 
** The communication speed of USBtiny programmer should be low enough maching w/128K MCU
avrdude -B 250 -c usbtiny -P usb -p attiny13 -U flash:w:"ISR-FuseBit.hex":i
avrdude -B 250 -c usbtiny -P usb -p attiny13 -U lfuse:w:0x6A:m flash:w:"ISR-FuseBit.hex":i

** To recover 9.6Mhz OSC, follow the two steps in a row
avrdude -B 250 -c usbtiny -P usb -p attiny13 -U lfuse:w:0x6A:m flash:w:"ISR-FuseBit.hex":i
avrdude -c usbtiny -P usb -p attiny13 -U flash:w:"ISR-FuseBit.hex":i

 Ref:
 #8 Posted by david.prentice: Sat. Feb 20, 2010 - 09:00 AM
 https://www.avrfreaks.net/forum/cannot-program-attiny13-any-more-slow-clock
*****************************************************************/
#define F_CPU 128000L

//#define F_CPU 1200000L

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

// ATtiny13A port usage
#define LED0 PB0

// AVR software reset macro
#define soft_reset()        \
do                          \
{                           \
	wdt_enable(WDTO_15MS);  \
	for(;;)                 \
	{                       \
	}                       \
} while(0)

// Watch Dog Timer period [sec]
//#define UNIT_DELAY_WDT 1 
//#define UNIT_DELAY_WDT 4 
#define UNIT_DELAY_WDT 8

// accumulated WDT period [EA] to last LED ON. 
// total duration of LED ON [sec] = UNIT_DELAY_WDT * SET_DELAY_UNIT
//#define SET_DELAY_UNIT 3
#define SET_DELAY_UNIT 15

uint8_t i;

uint8_t WDTtick=0; 

//-------- function prototypes --------
void systemInit(void);
void WDTsetup(void);
void toggleLED0(void);
void blinkLEDcnt(uint8_t );

//-------- interrupt service routines --------
ISR(WDT_vect)
{
	// --------- INCREASE WDT TICK COUNT ----------------------
	// increase WDTtick every UNIT_DELAY_WDT sec
	++WDTtick;
	//toggleLED0();
}//ISR(WDT_vect)


int main(void)
{
    systemInit();
	//WDTsetup();

	// Use the Power Down sleep mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	
	// ADC noise reduction sleep mode
	//set_sleep_mode(SLEEP_MODE_ADC);
	//blinkLEDcnt(10);
	while (1) 
	{
		// go to sleep and wait for interrupt...
		//sleep_mode();
		toggleLED0();
		//_delay_ms(50);

	}//while (1) 
}//main


//Not so perfect if using WDT isr with _delay_ms
//	for maybe there's only one timer circuit in ATtiny13a
void blinkLEDcnt(uint8_t num)
{
	uint8_t i;

	for(i=0; i<num; i++)
	{
		PORTB ^= (1<<LED0); // toggle on/off LED
		_delay_ms(500);
	}
	PORTB &= ~(1<<LED0); //off LED
	_delay_ms(500);
}//blinkLEDcnt

void toggleLED0(void)
{
	PORTB ^= (1<<LED0);	
}//toggleLED0

void systemInit(void)
{
	DDRB |= (1<<LED0); //output for LEDs
	PORTB &= ~(1<<LED0); //off LED

	//sei();          // enable all interrupts
}//systemInit

void WDTsetup(void)
{
	// set timer prescaler to UNIT_DELAY_WDT seconds
	// datasheet p43.
	switch (UNIT_DELAY_WDT)
	{
		case 50:
		WDTCR |= (1<<WDP2) | (1<<WDP0); // 0.5s
		break;
		case 1:
		WDTCR |= (1<<WDP2) | (1<<WDP1); // 1s
		break;
		case 4:
		WDTCR |= (1<<WDP3); // 4s
		break;
		case 8:
		WDTCR |= (1<<WDP3) | (1<<WDP0); // 8s
		break;
		default:
		WDTCR |= (1<<WDP3) | (1<<WDP0); // 8s
	}

	// Enable watchdog timer interrupts
	WDTCR |= (1<<WDTIE);
}//WDTsetup


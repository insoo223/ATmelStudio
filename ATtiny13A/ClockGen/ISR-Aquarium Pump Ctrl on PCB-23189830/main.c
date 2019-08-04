/**************************************************************
 Target MCU & clock speed: ATtiny13A @ 1.2Mhz internal
 Name    : main.c (copied from main_60minWDT_NoEEPROM.c)
 C modules of this project, ISR:
	main.c
 Custom Headers:
    Nothing
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : May 15, 2015
 Updated : Aug 04, 2019 (On Atmel Studio 7)

 Description: 
	ATtiny13A controls power up or down to aquarium pump by 2n2222 NPN transistor & relay.
	ATtiny13A sleeps in most of operation time and wake up periodically.
	Watch Dog Timer(WDT) wakes up & sleep periodically, and its period is defined by the followings:
		WDT Period [sec]  = UNIT_DELAY_WDT [sec] * SET_DELAY_UNIT	* WAKEUP_PERIOD	

 Tool to upload hex file to the target MCU(ATtiny13A)
	USB Tiny ISP Programmer
 
 Limitation:
	The WDT period can be influenced by ambient temperature. Hot weather can lengthen the period.
	
 HEX size[Byte]: 190 out of 1024

	avrdude.exe: AVR device initialized and ready to accept instructions

	Reading | ################################################## | 100% 0.00s

	avrdude.exe: Device signature = 0x1e9007
	avrdude.exe: NOTE: FLASH memory has been specified, an erase cycle will be performed
	To disable this feature, specify the -D option.
	avrdude.exe: erasing chip
	avrdude.exe: reading input file "ISR.hex"
	avrdude.exe: writing flash (190 bytes):

	Writing | ################################################## | 100% 0.25s

	avrdude.exe: 190 bytes of flash written
	avrdude.exe: verifying flash memory against ISR.hex:
	avrdude.exe: load data flash data from input file ISR.hex:
	avrdude.exe: input file ISR.hex contains 190 bytes
	avrdude.exe: reading on-chip flash data:

	Reading | ################################################## | 100% 0.17s

	avrdude.exe: verifying ...
	avrdude.exe: 190 bytes of flash verified

	avrdude.exe: safemode: Fuses OK

	avrdude.exe done.  Thank you. 

 How to upload to the target MCU
 <For Windows Atmel Studio 7.0>
	 Select Tool -> USBtiny (USBtiny memu should be configured in the external tool menu)
		Title: USBtin&Y
		Command: C:\WinAVR-20100110\bin\avrdude.exe
		Parameter: avrdude -c usbtiny -P usb -p attiny13 -U flash:w:ISR.hex:i
		Directory:$(ProjectDir)\Debug
 <For CMD window or DOS prompt>
	 cd "C:\Users\insoo\Documents\GitHub\ATmelStudio\ATtiny13A\ClockGen\ISR-Aquarium Pump Ctrl on PCB-23189830\Debug"
	 avrdude -c usbtiny -P usb -p attiny13 -U flash:w:ISR.hex:i

 Ref:
*****************************************************************/

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define TEST_MODE
//#define PRODUCTION_MODE

#ifdef TEST_MODE
	#define UNIT_DELAY_WDT	1 //<=== TEST VALUE in development;
	#define SET_DELAY_UNIT	3
	#define WAKEUP_PERIOD	2 
#endif
#ifdef PRODUCTION_MODE
	#define UNIT_DELAY_WDT	8 //<=== SELECTED VALUE in production; WDT period in seconds
	#define SET_DELAY_UNIT	225 
	#define WAKEUP_PERIOD	1 
							// 1: 30 min 2:one hour, 4:two hours,
							// when SET_DELAY_UNIT is 225 of ATtiny13a at 1.2Mhz
#endif

// # of UNIT_DELAY_WDT, Max 253
//#define SET_DELAY_UNIT 2	
//#define SET_DELAY_UNIT 15  // 2 min when UNIT_DELAY_WDT is 8
//#define SET_DELAY_UNIT 150 // 20 min when UNIT_DELAY_WDT is 8
//#define SET_DELAY_UNIT 225 //<=== SELECTED VALUE in production; about 30 min when UNIT_DELAY_WDT is 8
//#define SET_DELAY_UNIT 253 // (34 min - 8 sec) when UNIT_DELAY_WDT is 8

#define NPN_TR_PORT PB0 

uint8_t WDTtick; 
uint8_t WDTtick30min;

ISR(WDT_vect) 
{
	// --------- INCREASE WDT TICK COUNT ----------------------
	// increase WDTtick every UNIT_DELAY_WDT sec
	++WDTtick;

	// On every SET_DELAY_UNIT (half-hour) except last half-hour
	//  reset WDTtick count and increase half-hour count
	if ((WDTtick >= SET_DELAY_UNIT) && (WDTtick30min < WAKEUP_PERIOD))
	{
		// Reset WDT counter
		WDTtick = 0;

		//Increase WDT Half-hour counter
		++WDTtick30min;
	}//if (WDTtick >= SET_DELAY_UNIT)  && (WDTtick30min < WAKEUP_PERIOD)

	// On every WAKEUP_PERIOD (i.e. one hour)
	if (WDTtick30min >= WAKEUP_PERIOD)
	{
		PORTB ^= 1<<NPN_TR_PORT; //toggle on/off NPN TR
		// Reset WDT counter
		WDTtick = 0;
		//Reset WDT Half-hour counter
		WDTtick30min=0;
	}//if (WDTtick30min >= WAKEUP_PERIOD)
}//ISR(WDT_vect) 

int main(void) {

	WDTtick=0;
	WDTtick30min=0;
	// Set up NPN_TR_PORT mode to output
	DDRB = (1<<NPN_TR_PORT);

	// temporarily prescale timer to UNIT_DELAY_WDT seconds so we can measure current
	switch (UNIT_DELAY_WDT)
	{
		case 1:
			WDTCR |= (1<<WDP2) | (1<<WDP1); // 1s
			break;
		case 2:
			WDTCR |= (1<<WDP2) | (1<<WDP1) | (1<<WDP0); // 2s
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

	sei(); // Enable global interrupts

	// Use the Power Down sleep mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	PORTB = 1<<NPN_TR_PORT; //turn on NPN TR

	for (;;) {
		sleep_mode();   // go to sleep and wait for interrupt...
	}
}//main


/**************************************************************
 Target MCU & clock speed: ATtiny13A @ 1.2Mhz internal
 Name    : main.c 
 C modules of this project, ISR:
	main.c
 Custom Headers:
    Nothing
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : Aug 05, 2019 (On Atmel Studio 7)
 Updated : Aug 07, 2019 

 Description: 
	Aug 07, 2019 
		Add slide switch operation

	Aug 05, 2019 
	ATtiny13A controls power up or down to a summer fan by 2n2222 NPN transistor & relay.
	ATtiny13A sleeps in most of operation time and wake up periodically.
	Watch Dog Timer(WDT) wakes up & sleep periodically, and its period is defined by the followings:
		WDT Period [sec]  = UNIT_DELAY_WDT [sec] * SET_DELAY_UNIT	* WAKEUP_PERIOD	
	After about 2 hours of operation, the system will stop all the operation by disabling interrupts.
		This is for safety purpose.

 Tool to upload hex file to the target MCU(ATtiny13A)
	USB Tiny ISP Programmer
 
 Limitation:
	The WDT period can be influenced by ambient temperature. Hot weather can lengthen the period.
	
 HEX size[Byte]: 354 out of 1024
 
 How to upload to the target MCU
 <For Windows Atmel Studio 7.0>
	 Select Tool -> USBtiny (USBtiny memu should be configured in the external tool menu)
		Title: USBtin&Y
		Command: C:\WinAVR-20100110\bin\avrdude.exe
		Parameter: avrdude -c usbtiny -P usb -p attiny13 -U flash:w:ISR.hex:i
		Directory:$(ProjectDir)\Debug
 <For CMD window or DOS prompt>
	 cd "C:\Users\insoo\Documents\GitHub\ATmelStudio\ATtiny13A\ClockGen\ISR-Summer Fan Ctrl on PCB and Slide Switches23199832\Debug"
	 avrdude -c usbtiny -P usb -p attiny13 -U flash:w:ISR.hex:i

 Ref:
*****************************************************************/

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

//#define TEST_MODE
#define PRODUCTION_MODE

#ifdef TEST_MODE
	uint8_t UNIT_DELAY_WDT	= 1; //<=== TEST VALUE in development;
	uint8_t SET_DELAY_UNIT	= 1;
	uint8_t WAKEUP_PERIOD	= 1;
	uint8_t SYS_STOP_PERIOD	= 2*10-1; //system stop after turning on 5 times, i.e. about 10sec
#endif
#ifdef PRODUCTION_MODE
	uint8_t UNIT_DELAY_WDT	= 8; //<=== SELECTED VALUE in production; WDT period in seconds
	uint8_t SET_DELAY_UNIT	= 7; 
	uint8_t WAKEUP_PERIOD	= 3; 
							// 1: 56sec (about 1min) 2:112sec 3:168sec 
							// when SET_DELAY_UNIT is 7 of ATtiny13a at 1.2Mhz
	uint8_t SYS_STOP_PERIOD	= 2*20-1; //system stop after turning on 20 times, i.e. about 2hour
#endif

// # of UNIT_DELAY_WDT, Max 253
// SET_DELAY_UNIT 2	
// SET_DELAY_UNIT 15  // 2 min when UNIT_DELAY_WDT is 8
// SET_DELAY_UNIT 150 // 20 min when UNIT_DELAY_WDT is 8
// SET_DELAY_UNIT 225 //<=== SELECTED VALUE in production; about 30 min when UNIT_DELAY_WDT is 8
// SET_DELAY_UNIT 253 // (34 min - 8 sec) when UNIT_DELAY_WDT is 8

#define NPN_TR_PORT PB0 //Insoo designed PCB(PN 23189811) with a relay & four slide switches(three for modes, one for pwr)
#define LEFT_SW_PORT PB3 //Left-most slide switch input port
#define MID_SW_PORT PB1 //Middle slide switch input port
#define RITE_SW_PORT PB2 //Right-most slide switch input port

uint8_t WDTtick; //sec counter of UNIT_DELAY_WDT
uint8_t WDTtick1min; //sec counter of UNIT_DELAY_WDT * SET_DELAY_UNIT
uint8_t WDTtick3min; //sec counter of UNIT_DELAY_WDT * SET_DELAY_UNIT * WAKEUP_PERIOD
uint8_t leftSWstatus;

ISR(WDT_vect) 
{
	// --------- INCREASE WDT TICK COUNT ----------------------
	// increase WDTtick every UNIT_DELAY_WDT sec
	++WDTtick;
	//check LEFT switch on PB
	if (PINB & 0x08)
		WAKEUP_PERIOD = 1;
	//check Mid switch on PB
	if (PINB & 0x02)
		WAKEUP_PERIOD = 2;
	if (PINB & 0x04)
		WAKEUP_PERIOD = 3;
	// On every SET_DELAY_UNIT,
	//  reset WDTtick count and increase 1min count
	if ((WDTtick >= SET_DELAY_UNIT) && (WDTtick1min < WAKEUP_PERIOD))
	{
		// Reset WDT 8sec counter
		WDTtick = 0;

		//Increase WDT 1min counter
		++WDTtick1min;
	}//if (WDTtick >= SET_DELAY_UNIT)  && (WDTtick1min < WAKEUP_PERIOD)

	// On every WAKEUP_PERIOD (i.e. 3min)
	if (WDTtick1min >= WAKEUP_PERIOD)
	{
		PORTB ^= 1<<NPN_TR_PORT; //toggle on/off NPN TR
		// Reset WDT 8sec counter
		WDTtick = 0;
		//Reset WDT 1min counter
		WDTtick1min=0;
		//Increase WDT 3min counter
		WDTtick3min++;
	}//if (WDTtick1min >= WAKEUP_PERIOD)

	// After 1hour-long operation, stop all the operation for safety measure.
	if (WDTtick3min >= SYS_STOP_PERIOD)
	{
		// Disable watchdog timer interrupts
		WDTCR &= ~(1<<WDTIE);
		// Disable global interrupts
		cli(); 
	}
}//ISR(WDT_vect) 

int main(void) {
	WDTtick=0;
	WDTtick1min=0;

	// Set up NPN_TR_PORT mode to output
	DDRB = (1<<NPN_TR_PORT);
	// Set up slide switches mode to intput
	DDRB &= ~(1<<LEFT_SW_PORT);
	DDRB &= ~(1<<MID_SW_PORT);
	DDRB &= ~(1<<RITE_SW_PORT);

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

void click()
{
	PORTB ^= 1<<NPN_TR_PORT;
	_delay_ms(500);
	PORTB ^= 1<<NPN_TR_PORT;
	_delay_ms(500);
}//click

void testConnectedPort()
{
	uint8_t relayPort, i;

	for (relayPort = PB0; relayPort < PB5; relayPort++)
	{
		DDRB = (1<<relayPort);
		for (i=PB0; i<=relayPort; i++)
		{
			PORTB ^= 1<<relayPort;
			_delay_ms(500);
		}
	}
}//testConnectedPort
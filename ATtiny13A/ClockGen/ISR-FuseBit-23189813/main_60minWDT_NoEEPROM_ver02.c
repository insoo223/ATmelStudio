/**************************************************************
 Target MCU & clock speed: ATtiny13A @ 1.2Mhz internal
 Name    : main.c
 C modules of this project, ISR:
main.c
Custom Headers:
    Nothing
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : May 15, 2015
 Updated : Sep 03, 2018 (On Atmel Studio 7)

 Description: 
	ATtiny13A controls 220V power relay for multiple purposes using three slide switches.
	In this particular application for managing submersed water pump in fish tank, 
	I don't care slide switches, but only works in a single mode of operation. 

 HEX size[Byte]: 218 out of 1024

How to upload to the target MCU
<For Windows Atmel Studio>
Slect Tool ? USBtiny (USBtiny memu should be configured in the external tool memu)

<For CMD window or DOS prompt>
cd " C:\Users\insoo\Documents\GitHub\ATmelStudio\ATtiny13A\ClockGen\ISR\Debug "
avrdude -c usbtiny -P usb -p attiny13 -U flash:w:ISR.hex:i

 Ref:
*****************************************************************/

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <util/delay.h>

//#define TEST_MODE
#define PRODUCTION_MODE

#ifdef TEST_MODE
	#define UNIT_DELAY_WDT 4 //<=== TEST VALUE in development; 4 sec WDT
	#define INITPUMP_DURATION 5000 // 5 sec for initial pump period
#endif
#ifdef PRODUCTION_MODE
	#define UNIT_DELAY_WDT 8 //<=== SELECTED VALUE in production; WDT period in seconds; 8 sec WDT
	#define INITPUMP_DURATION 10000 // 60 sec for initial pump period
#endif

//select accumulated WDT period as DELAY_UNIT = UNIT_DELAY_WDT * SET_DELAY_UNIT
#ifdef TEST_MODE 
	#define SET_DELAY_UNIT 2 //<=== TEST VALUE in development;  4*3=12 [sec]
#endif
#ifdef PRODUCTION_MODE
	#define SET_DELAY_UNIT 225 //<=== SELECTED VALUE in production; 8*225=1800[sec]=30 minutes
#endif
// # of UNIT_DELAY_WDT, Max 253
//#define SET_DELAY_UNIT 2	// <=== TEST VALUE in development
//#define SET_DELAY_UNIT 15  // 2 min (15*8=120[sec]) when UNIT_DELAY_WDT is 8
//#define SET_DELAY_UNIT 150 // 20 min when UNIT_DELAY_WDT is 8
//#define SET_DELAY_UNIT 225 //<=== SELECTED VALUE in production; 30 min when UNIT_DELAY_WDT is 8
//#define SET_DELAY_UNIT 253 // (34 min - 8 sec) when UNIT_DELAY_WDT is 8

//select accumulated DELAY_UNIT as WAKEUP period  = SET_DELAY_UNIT * WAKEUP_PERIOD
#ifdef TEST_MODE
	//4*3*1=12 [sec], every 12 sec system will wakeup from sleep and do ISR
	//#define WAKEUP_PERIOD	1 //<=== TEST VALUE in development; 
	uint8_t wakeupPeriod = 1;
#endif
#ifdef PRODUCTION_MODE
	//8*225*2=3600[sec]=1 [hour], every hour system will wakeup from sleep and do ISR
	//#define WAKEUP_PERIOD	2 //<=== SELECTED VALUE in production; 
								// 2:one hour, 4:two hours, etc
								// when SET_DELAY_UNIT is 225 of ATtiny13a at 1.2Mhz
	uint8_t wakeupPeriod = 2;   
#endif

//select WAKEUP duration,i.e. how long ISR function will persist.

#ifdef TEST_MODE
	//#define wakeupDuration 75 // 10 min operation, Refer the comments of SET_DELAY_UNIT
	//#define wakeupDuration 1 // 4 sec operation, Refer the comments of SET_DELAY_UNIT
	uint8_t wakeupDuration = 1; // 4 sec operation, Refer the comments of SET_DELAY_UNIT
#endif
#ifdef PRODUCTION_MODE
	//#define wakeupDuration 75 // 10 min operation, Refer the comments of SET_DELAY_UNIT
	//#define wakeupDuration 150 // 20 min operation, Refer the comments of SET_DELAY_UNIT
	uint8_t wakeupDuration = 150; // 4 sec operation, Refer the comments of SET_DELAY_UNIT
#endif

//port for the transistor to control relay on/off 
#define NPN_TR_PORT PB0 // when using NPN TR

uint8_t WDTtick; 
uint8_t WDTtick30min;
//uint8_t readBit;
ISR(WDT_vect) 
{
	//readBit = PINB & PB3 ;
	//if (readBit == 1)
	//if(bit_it_set(PINB, PB3))
	if (PINB & (1<<PB3))
		//wakeupDuration = 2;
		wakeupPeriod = 1; // every 30min in production mode
	else
		//wakeupDuration = 1;
		wakeupPeriod = 2; // every 60min in production mode

	// --------- INCREASE WDT TICK COUNT ----------------------
	// increase WDTtick every UNIT_DELAY_WDT sec
	++WDTtick;

	// On every SET_DELAY_UNIT (half-hour) except last half-hour
	//  reset WDTtick count and increase half-hour count
	if ((WDTtick >= SET_DELAY_UNIT) && (WDTtick30min < wakeupPeriod))
	{
		// Reset WDT counter value of the designated address in the EEPROM of ATtiny13A
		//eeprom_update_byte((uint8_t*)WDTTICK_CTR_ADDR, 0);
		WDTtick = 0;

		//Increase WDT Half-hour counter value of the designated address in the EEPROM of ATtiny13A
		//eeprom_update_byte((uint8_t*)WDTTICK_30MIN_ADDR, ++WDTtick30min);
		++WDTtick30min;
	}//if (WDTtick >= SET_DELAY_UNIT)  && (WDTtick30min < WAKEUP_PERIOD)

	// On every wakeupPeriod (i.e. one hour in production mode)
	if (WDTtick30min >= wakeupPeriod)
	{
		// --------- DO TO PROPER ACTION TO WDT TICK COUNT ----------------------
		// When the accumulated WDT reaches every SET_DELAY_UNIT, turn on ESP-01
		if (WDTtick == 0)
		{
			// Give logic HIGH to port 4 to turn ON NPN transistor(2n2222),
			//  so let the GND of ESP-01 module CONNECT to system GND.
			// This will power ON ESP-01 and measure temperature & humidity via DHT22
			PORTB = 1<<NPN_TR_PORT; //turn on GND of MOSFET or ESP-01
		}//if (WDTtick == 0)
		else if (WDTtick >= wakeupDuration)
		{
			// Give logic LOW to port 4 to turn OFF NPN transistor(2n2222),
			//  so let the GND of ESP-01 module DISCONNECT to system GND.
			// This will power OFF ESP-01 and don't measure temperature & humidity via DHT22
			PORTB = (0<<NPN_TR_PORT); //turn off GND of MOSFET or ESP-01

			// Reset WDT counter value of the designated address in the EEPROM of ATtiny13A
			//eeprom_update_byte((uint8_t*)WDTTICK_CTR_ADDR, 0);
			WDTtick = 0;
			//Reset WDT Half-hour counter value of the designated address in the EEPROM of ATtiny13A
			//eeprom_update_byte((uint8_t*)WDTTICK_30MIN_ADDR, 0);
			WDTtick30min=0;
		}//else if (WDTtick == 1)
	}//if (WDTtick30min >= wakeupPeriod)


}//ISR(WDT_vect) 

int main(void) {

	WDTtick=0;
	WDTtick30min=0;
	// Set NPN_TR_PORT mode to output
	DDRB = (1<<NPN_TR_PORT);
	// Set PB1-3 mode to input
	DDRB &= ~(1<<PB1);
	DDRB &= ~(1<<PB2);
	DDRB &= ~(1<<PB3);

	PORTB = 1<<NPN_TR_PORT; //turn on MOSFET

	_delay_ms(INITPUMP_DURATION); // initial measurement of DHT22 via ESP-01 for 10 sec
	
	PORTB = 0<<NPN_TR_PORT; //turn off MOSFET

	// temporarily prescale timer to UNIT_DELAY_WDT seconds so we can measure current
	switch (UNIT_DELAY_WDT)
	{
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
	//set_sleep_mode(SLEEP_MODE_IDLE);

	for (;;) {
		sleep_mode();   // go to sleep and wait for interrupt...
	}
}//main


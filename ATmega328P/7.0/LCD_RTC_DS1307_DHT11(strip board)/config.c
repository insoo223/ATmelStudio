/**************************************************************
 Target MCU & clock speed: ATmega328P @ 1Mhz internal
 Name    : config.c
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : 2017-07-16 오전 8:30:27
	Dispatched from main.c
 Updated : 
	Fri. Jan 25, 2019: 
	In function "RTC_DS1307config", set DS1307 RTC chip power turn on to save coin cell battery
	DS1307_VCC_port |= _BV(DS1307_VCC_bit);

 Description: 
	0) "defines.h" file has specific pin assignment info.
	1) configure MCU IO pins for LCD, button switch, DHT11, and debug pin.
	2) debug pin can be used with oscilloscope to check timing info.

 HEX size[Byte]:

 Ref:
 *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include "externs.h"
#include "defines.h"
#include <util/delay.h>

//------------------------------------
void config()
{
	LCDconfig();
	SwitchConfig();
	RTC_DS1307config();
	DHT11config();
	BuzzerConfig();
	debugPinConfig();
		
	// initialize the LCD controller as determined by the defines (LCD instructions)
	// initialize the LCD display for a 4-bit interface
	//lcd_init_4d();

}//config

/*---------------- 
PORTD for LCD
----------------*/
void LCDconfig()
{
// Data lines of nibble - Output
	lcd_D7_ddr |= _BV(lcd_D7_bit);
	lcd_D6_ddr |= _BV(lcd_D6_bit);
	lcd_D5_ddr |= _BV(lcd_D5_bit);
	lcd_D4_ddr |= _BV(lcd_D4_bit);

// E pin (Enable) - Output
	lcd_E_ddr |= _BV(lcd_E_bit);
	//turn off lcd_E
	lcd_E_port &= ~_BV(lcd_E_bit);


// RS pin (Register Select) - Output
	lcd_RS_ddr |= _BV(lcd_RS_bit);
	//turn off lcd_RS
	lcd_RS_port &= ~_BV(lcd_RS_bit);

// Back light cathode pin (K) - Output
	lcd_Backlight_ddr |= _BV(lcd_Backlight_bit);
	// Turn off back light
	// for it is GND or cathode pin, HIGH make it off
	lcd_Backlight_port |= _BV(lcd_Backlight_bit);

// VDD pin (Power Supply) - Output
	// added by Insoo (Jul 11, 2017) to control LCD power consumption while sleep
	lcd_VDD_ddr |= _BV(lcd_VDD_bit);
	//turn off VDD
	//lcd_VDD_port &= ~_BV(lcd_VDD_bit);

}//LCDconfig

/*----------------
PORTB for the button/tactile switch
----------------*/
void SwitchConfig()
{
	//Tactile switch1 - Input
	tactile_Switch_ddr &= ~_BV(tactile_Switch_bit);

	//Tactile switch2 - Input
	tactile_Switch2_ddr &= ~_BV(tactile_Switch2_bit);
}//SwitchConfig

/*----------------
Real-Time Clock DS1307 chip
----------------*/
void RTC_DS1307config()
{
	// DS1307 VCC pin (Power Supply) - Output
	//added by Insoo (Jul 11, 2017)
	DS1307_VCC_ddr |= _BV(DS1307_VCC_bit);
	//turn off DS1307 RTC chip power
	//DS1307_VCC_port &= ~_BV(DS1307_VCC_bit);
	//turn on DS1307 RTC chip power to save coin cell battery
	DS1307_VCC_port |= _BV(DS1307_VCC_bit);

	//added by Insoo (Jan 30, 2019)
	DS1307_SQW_ddr &= ~_BV(DS1307_SQW_bit); //Input for Square Wave signal from DS1307
}//RTC_DS1307config

/*----------------
PORTB for the DHT11 temp/humidity sensor
----------------*/
void DHT11config()
{
	// DHT11 (temp & humid sensor) VCC pin (Power Supply) - Output
	//added by Insoo (Jul 14, 2017)
	DHT_VCC_ddr |= _BV(DHT_VCC_bit);
	//turn off DS1307 RTC chip power
	DHT_VCC_port &= ~_BV(DHT_VCC_bit);
}//DHT11config

void BuzzerConfig()
{
	//Buzzer - Output
	BUZZER_ddr |= _BV(BUZZER_bit);
	//turn off buzzer
	BUZZER_port &= ~_BV(BUZZER_bit);

}//BuzzerConfig


/*----------------
PORTB for the debug pin 
----------------*/
void debugPinConfig()
{
	// debug pin - Output
	debug_PIN_ddr |= _BV(debug_PIN_bit);
	//turn off debug_PIN_bit
	debug_PIN_port &= ~_BV(debug_PIN_bit);
	//turn on debug_PIN_bit
	//debug_PIN_port |= _BV(debug_PIN_bit);
}//debugPinConfig

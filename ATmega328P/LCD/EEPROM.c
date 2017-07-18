  /**************************************************************
 Target MCU & clock speed: ATmega328P @ 1Mhz internal
 Name    : EEPROM.c
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : 2017-07-17 오전 7:39:09
 Updated : 

 Description: EEPROM handling
	1) Use AVR library for EEPROM

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

#include <avr/eeprom.h> 

uint8_t ew_ptr=0;
uint8_t er_ptr=0;

//-----------------------------------
void ER_Byte_LCD(uint8_t rptr)
{
	uint8_t ByteOfData;
	char strNum[3];

	ByteOfData = eeprom_read_byte((uint8_t*)rptr);

	itoa(ByteOfData, strNum, 10);
	lcd_write_string_4d((uint8_t*)strNum);
	_delay_ms(1000);
	LCD_blankChar(1);
}//ER_Byte_LCD

//-----------------------------------
void EW_tempHumid()
{
	//eeprom_update_byte((uint8_t*)(addr++), i);
	eeprom_update_byte((uint8_t*)0, temp);
	_delay_ms(100);
	eeprom_update_byte((uint8_t*)1, humid);
	_delay_ms(100);
}//EW_tempHumid

//-----------------------------------
void EW_Time()
{
	byte *hours = 0, *minutes = 0, *seconds = 0;

	DS1307_GetTime(hours, minutes, seconds);
	_delay_ms(500);
	DS1307_GetTime(hours, minutes, seconds);
	//eeprom_update_byte((uint8_t*)(addr++), i);
	eeprom_update_byte((uint8_t*)0, *hours);
	_delay_ms(100);
	eeprom_update_byte((uint8_t*)1, *minutes);
	_delay_ms(100);
	eeprom_update_byte((uint8_t*)2, *seconds);
	_delay_ms(100);
}//EW_tempHumid
﻿/*
 * DS1307.c
 *
 * Created: 2017-07-16 오전 11:43:30
 *  Author: insoo
 */ 
 /**************************************************************
 Target MCU & clock speed: ATmega328P @ 1Mhz internal
 Name    : DS1307.c
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : 2017-07-16 오전 11:43:30
 Updated : 

 Description: RTC functions using DS1307 chip

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

//-----------------------------------
void setTime2DS1307(byte wkDay,byte month,byte day,byte year, byte ampm, byte h, byte m, byte s)
{
	makePDasOutput();
	//turn on DS1307 power
	DS1307_VCC_port |= _BV(DS1307_VCC_bit);
	_delay_ms(10);
	SetTimeDate(wkDay, month, day, year,  ampm,  h,  m,  s);
	_delay_ms(1000);
	
	LCD_TimeDate();
	_delay_ms(3000);
}//setTime2DS1307

//-----------------------------------
// ---------------------------------------------------------------------------
// DS1307 RTC ROUTINES
#define DS1307 0xD0 // I2C bus address of DS1307 RTC
#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x01
#define HOURS_REGISTER 0x02
#define DAYOFWK_REGISTER 0x03
#define DAYS_REGISTER 0x04
#define MONTHS_REGISTER 0x05
#define YEARS_REGISTER 0x06
#define CONTROL_REGISTER 0x07
#define RAM_BEGIN 0x08
#define RAM_END 0x3F

void DS1307_GetTime(byte *hours, byte *minutes, byte *seconds)
// returns hours, minutes, and seconds in BCD format
{
	*hours = I2C_ReadRegister(DS1307,HOURS_REGISTER);
	*minutes = I2C_ReadRegister(DS1307,MINUTES_REGISTER);
	*seconds = I2C_ReadRegister(DS1307,SECONDS_REGISTER);
	if (*hours & 0x40) // 12hr mode:
	*hours &= 0x1F; // use bottom 5 bits (pm bit = temp & 0x20)
	else *hours &= 0x3F; // 24hr mode: use bottom 6 bits
}//DS1307_GetTime

void DS1307_GetDate(byte *months, byte *days, byte *years)
// returns months, days, and years in BCD format
{
	*months = I2C_ReadRegister(DS1307,MONTHS_REGISTER);
	*days = I2C_ReadRegister(DS1307,DAYS_REGISTER);
	*years = I2C_ReadRegister(DS1307,YEARS_REGISTER);
}//DS1307_GetDate

void DS1307_GetWkDay(byte *wkdays)
// returns days of week in BCD format
{
	*wkdays = I2C_ReadRegister(DS1307,DAYOFWK_REGISTER);
}//DS1307_GetWkDay

void SetTimeDate(byte wkDay,byte month,byte day,byte year, byte ampm, byte h, byte m, byte s)
// simple, hard-coded way to set the date.
{
	//I2C_WriteRegister(DS1307,DAYOFWK_REGISTER, 0x06); //Wednesday
	I2C_WriteRegister(DS1307,DAYOFWK_REGISTER, wkDay); //Wednesday
	_delay_ms(10);

	//I2C_WriteRegister(DS1307,MONTHS_REGISTER, 0x07);
	I2C_WriteRegister(DS1307,MONTHS_REGISTER, month);
	//When F_CPU is 1Mhz , delay 10ms is required
	//as of July 11, 2017 (insoo tested and succeeded)
	_delay_ms(10);
	//I2C_WriteRegister(DS1307,DAYS_REGISTER, 0x15);
	I2C_WriteRegister(DS1307,DAYS_REGISTER, day);
	_delay_ms(10);
	//I2C_WriteRegister(DS1307,YEARS_REGISTER, 0x17);
	I2C_WriteRegister(DS1307,YEARS_REGISTER, year);
	_delay_ms(10);
	//I2C_WriteRegister(DS1307,HOURS_REGISTER, 0x11); // add 0x40 for PM
	//I2C_WriteRegister(DS1307,HOURS_REGISTER, 0x05+0x40); // add 0x40 for PM
	if (ampm == PM)
	I2C_WriteRegister(DS1307,HOURS_REGISTER, h+0x40); // add 0x40 for PM
	else
	I2C_WriteRegister(DS1307,HOURS_REGISTER, h); // add 0x40 for PM
	_delay_ms(10);

	//I2C_WriteRegister(DS1307,MINUTES_REGISTER, 0x38);
	I2C_WriteRegister(DS1307,MINUTES_REGISTER, m);
	_delay_ms(10);
	//I2C_WriteRegister(DS1307,SECONDS_REGISTER, 0x30);
	I2C_WriteRegister(DS1307,SECONDS_REGISTER, s);
	_delay_ms(10);
}//SetTimeDate
//-----------------------------------
//-----------------------------------

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
	
	LCD_WriteTimeDate();
	_delay_ms(3000);
}//setTime2DS1307

//-----------------------------------
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

//-----------------------------------
void DS1307_GetDate(byte *months, byte *days, byte *years)
// returns months, days, and years in BCD format
{
	*months = I2C_ReadRegister(DS1307,MONTHS_REGISTER);
	*days = I2C_ReadRegister(DS1307,DAYS_REGISTER);
	*years = I2C_ReadRegister(DS1307,YEARS_REGISTER);
}//DS1307_GetDate

//-----------------------------------
void DS1307_GetWkDay(byte *wkdays)
// returns days of week in BCD format
{
	*wkdays = I2C_ReadRegister(DS1307,DAYOFWK_REGISTER);
}//DS1307_GetWkDay

//-----------------------------------
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
void adjustHour()
{
	uint8_t DONE=0, val;
	uint8_t curLoop=0, preLoop=0, lapse=0;
	char strHour[3];

	while (!DONE)
	{
		// set cursor to start of first line
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
		_delay_us(DELAY_INST);                                  // 40 uS delay (min)
		// display the first line of information
		lcd_write_string_4d((uint8_t *)"Hour: ");
		itoa(hour, strHour, 10);
		lcd_write_string_4d((uint8_t *)strHour);
		lcd_write_string_4d((uint8_t *)" ");
		val = tactile_Switch_port & _BV(tactile_Switch_bit);
		_delay_ms(100);
		if (val == 0)
		{
			hour++;
			if (hour > 23)
			hour=0;
			preLoop = curLoop;
		}
		curLoop++;
		lapse = curLoop - preLoop;
		if (lapse > adjustTimeInterval)
		DONE = 1;
		_delay_ms(halfSec/4);
	}//while (!DONE)
}//adjustHour

//-----------------------------------
void adjustMin()
{
	uint8_t DONE=0, val;
	uint8_t curLoop=0, preLoop=0, lapse=0;
	char strMin[3];

	while (!DONE)
	{
		// set cursor to start of first line
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
		_delay_us(DELAY_INST);                                  // 40 uS delay (min)
		// display the first line of information
		lcd_write_string_4d((uint8_t *)"Min: ");
		itoa(min, strMin, 10);
		lcd_write_string_4d((uint8_t *)strMin);
		lcd_write_string_4d((uint8_t *)" ");
		val = tactile_Switch_port & _BV(tactile_Switch_bit);
		_delay_ms(100);
		if (val == 0)
		{
			min++;
			if (min > 59)
			min=0;
			preLoop = curLoop;
		}
		curLoop++;
		lapse = curLoop - preLoop;
		if (lapse > adjustTimeInterval)
		DONE = 1;
		_delay_ms(halfSec/4);
	}//while (!DONE)

}//adjustMin

//-----------------------------------
void adjustSec()
{
	uint8_t DONE=0, val;
	uint8_t curLoop=0, preLoop=0, lapse=0;
	char strSec[3];

	while (!DONE)
	{
		// set cursor to start of first line
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
		_delay_us(DELAY_INST);                                  // 40 uS delay (sec)
		// display the first line of information
		lcd_write_string_4d((uint8_t *)"sec: ");
		itoa(sec, strSec, 10);
		lcd_write_string_4d((uint8_t *)strSec);
		lcd_write_string_4d((uint8_t *)" ");
		val = tactile_Switch_port & _BV(tactile_Switch_bit);
		_delay_ms(100);
		if (val == 0)
		{
			sec++;
			if (sec > 59)
			sec=0;
			preLoop = curLoop;
		}
		curLoop++;
		lapse = curLoop - preLoop;
		if (lapse > adjustTimeInterval)
		DONE = 1;
		_delay_ms(halfSec/4);
	}//while (!DONE)

}//adjustSec


//-----------------------------------

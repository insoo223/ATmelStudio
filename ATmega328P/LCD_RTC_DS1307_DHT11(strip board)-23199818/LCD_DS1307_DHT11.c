/*************************************
Target MCU & clock speed: ATmega328P @ 1Mhz internal
Name    : LCD_DS1307_DHT11.c
Author  : Insoo Kim (insoo@hotmail.com)
Created : 2017-07-16 오전 11:59:50
Updated : 

Description: RTC functions using DS1307 chip

HEX size[Byte]:

Ref:
**************************************/

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

//----------------------------------
void ShowI2Cdevices()
// Scan I2C addresses and display addresses of all devices found
{
	//LCD_Line(1); LCD_String("Found:");
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	lcd_write_string_4d((uint8_t *)"Found:");

	byte addr = 1;
	while (addr>0)
	{
		//LCD_Char(' ');
		lcd_write_character_4d(' ');

		addr = I2C_FindDevice(addr);
		if (addr>0)
		LCD_Hex(addr++);
	}
}//ShowDevices


//----------------------------------
void LCD_WriteDate()
{
	byte months, days, years;
	byte wkDays;

	DS1307_GetDate(&months,&days,&years);
	LCD_BCDDigits(months);
	//LCD_Char('/');
	lcd_write_character_4d('/');
	LCD_BCDDigits(days);
	//LCD_Char('/');
	lcd_write_character_4d('/');
	LCD_BCDDigits(years);

	lcd_write_character_4d(' ');
	DS1307_GetWkDay(&wkDays);
	//LCD_BCDDigits(wkDays); // Day as number
	dispDayAsStr(wkDays);

}//LCD_WriteDate

/*
Ref) String handling
 https://www.nongnu.org/avr-libc/user-manual/group__avr__string.html#ga54e4f23104fa6f722f9459d2673a1eba
*/
void dispDayAsStr(byte wkDays)
{
	char strWkDays[5];

	switch(wkDays)
	{
		case 1: strcpy(strWkDays,"Mon");
				break;
		case 2: strcpy(strWkDays,"Tue");
				break;
		case 3: strcpy(strWkDays,"Wed");
				break;
		case 4: strcpy(strWkDays,"Thu");
				break;
		case 5: strcpy(strWkDays,"Fri");
				break;
		case 6: strcpy(strWkDays,"Sat");
				break;
		case 7: strcpy(strWkDays,"Sun");
				break;
		default:
				strcpy(strWkDays,"???");
	}
	lcd_write_string_4d((uint8_t *)strWkDays);


}
//----------------------------------
void LCD_WriteTime()
{
	byte hours, minutes, seconds;
	DS1307_GetTime(&hours,&minutes,&seconds);
	LCD_BCDDigits(hours);
	//LCD_Char(':');
	lcd_write_character_4d(':');

	LCD_BCDDigits(minutes);
	//LCD_Char(':');
	lcd_write_character_4d(':');

	LCD_BCDDigits(seconds);
}//LCD_WriteTime

//----------------------------------
void LCD_WriteTimeDate()
{
	//LCD_Line(0);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	LCD_WriteTime();
	LCD_blankChar(8);

	//LCD_Line(1);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	

	LCD_blankChar(8);
}//LCD_WriteTimeDate

//----------------------------------
void LCD_WriteTimeDateTempHumid()
{
	//LCD_Line(0);
	getDHT();
	//_delay_ms(1000);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	LCD_WriteTime();
	LCD_blankChar(5);
	lcd_write_string_4d((uint8_t *)strTemp);
	//LCD_blankChar(1);
	lcd_write_character_4d('C');

	//LCD_Line(1);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	LCD_WriteDate();
	//LCD_blankChar(2); // For display WkDay as number
	LCD_blankChar(1); // For display WkDay as string
	lcd_write_string_4d((uint8_t *)strHumid);
	//LCD_blankChar(1);
	//lcd_write_character_4d('%');
	lcd_write_character_4d(37);
}//LCD_WriteTimeDateTempHumid

//----------------------------------
void LCD_WriteWelcome()
{
	prepareWakeUpandLCDHome();

	//LCD_Line(0);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	lcd_write_string_4d((uint8_t *)"Welcome Insoo's!");
	//LCD_blankChar(5);

	//LCD_Line(1);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	lcd_write_string_4d((uint8_t *)"built Jan29,2019");
	LCD_blankChar(2);

	_delay_ms(3000);
	//makePDasInput();
	
}//LCD_WriteWelcome

//-- Welcome message
void powerOnWelcomeMessage()
{
	prepareWakeUpandLCDHome();
	buzzerBeepBeep();
	LCD_WriteWelcome();
	_delay_ms(2000);
	lcd_dispMenu();
	_delay_ms(2000);
	LCD_WriteTimeDateTempHumid();
	_delay_ms(2000);
	makePDasInput(); //make LCD in deep power save mode
}//powerOnWelcomeMessage

void powerOnWelcomeMessage_Short()
{
	//prepareWakeUpandLCDHome();
	//buzzerBeepBeep();
	LCD_WriteWelcome();
	_delay_ms(3000);
	makePDasInput(); //make LCD in deep power save mode
}//powerOnWelcomeMessage_Short

void showPowerOnTime()
{
	//prepareWakeUpandLCDHome();
	//buzzerBeepBeep();
	LCD_WriteTimeDateTempHumid();
	_delay_ms(3000);
	makePDasInput(); //make LCD in deep power save mode
}//showPowerOnTime

//----------------------------------
void test_LCD_DS1307()
{
	//------------test LCD DS1307
	//ShowI2Cdevices();
	//LCD_BCDDigits(0x37);
	//LCD_WriteDate();
	//LCD_WriteTimeDate();
	//_delay_ms(1000);

	//sysClockTest();
	//lcd_dispRealClock();
	//lcd_showDHT11();
	//_delay_ms(1000);
	//lcd_dispProgInfo();
	//_delay_ms(1000);

	//testTimingOnDebugPin()
}//test_LCD_DS1307
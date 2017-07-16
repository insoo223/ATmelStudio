/*
 * DS1307onLCD.c
 *
 * Created: 2017-07-16 오전 11:59:50
 *  Author: insoo
 */ 

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
	LCD_BCDDigits(wkDays);

}//LCD_WriteDate

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
void LCD_TimeDate()
{
	//LCD_Line(0);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	LCD_WriteTime();
	LCD_blankChar(8);

	//LCD_Line(1);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	LCD_WriteDate();
	LCD_blankChar(8);
}//LCD_TimeDate

//----------------------------------
void LCD_TimeDateTempHumid()
{
	//LCD_Line(0);
	getDHT();
	//_delay_ms(1000);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	LCD_WriteTime();
	LCD_blankChar(1);
	lcd_write_string_4d((uint8_t *)strTemp);
	//LCD_blankChar(1);
	lcd_write_character_4d('C');

	//LCD_Line(1);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	LCD_WriteDate();
	LCD_blankChar(2);
	lcd_write_string_4d((uint8_t *)strHumid);
	//LCD_blankChar(1);
	//lcd_write_character_4d('%');
	lcd_write_character_4d(37);
}//LCD_TimeDateTempHumid

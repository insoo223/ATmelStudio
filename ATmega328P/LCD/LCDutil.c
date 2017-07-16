/**************************************
Target MCU & clock speed: ATmega328P @ 1Mhz internal
Name    : LCDutil.c
Author  : Insoo Kim (insoo@hotmail.com)
Created : 2017-07-16 오전 11:49:29
Dispatched from main.c file.

Updated : 
	
Description: 

HEX size[Byte]:

Ref:
Donald Weiman    (weimandn@alfredstate.edu)
Summary:    4-bit data interface, busy flag not implemented.
	Any LCD pin can be connected to any available I/O port.
	Includes a simple write string routine.
http://web.alfredstate.edu/weimandn/programming/lcd/ATmega328/LCD_code_gcc_4d.html
*************************************/

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
void LCD_Hex(int data)
// displays the hex value of DATA at current LCD cursor position
{
	char st[8] = ""; // save enough space for result
	itoa(data,st,16); // convert to ascii hex
	//LCD_Message("0x"); // add prefix "0x" if desired
	//LCD_String(st); // display it on LCD
	lcd_write_string_4d((uint8_t *)st);
}//LCD_Hex

//-----------------------------------
void LCD_BCDDigits(uint8_t data)
// helper function for LCD_WriteDate()
// input is two digits in BCD format
// output is to LCD display at current cursor position
{
	byte temp = data>>4;
	//LCD_Char(temp+'0');
	lcd_write_character_4d(temp+'0');
	data &= 0x0F;
	//LCD_Char(data+'0');
	lcd_write_character_4d(data+'0');
}//LCD_BCDDigits

//-----------------------------------
void turnOnLCDBacklight()
{
	//turn on LCD backlight
	//  by giving 0 volt to K of LCD
	lcd_Backlight_port &= ~_BV(lcd_Backlight_bit);
}//turnOnLCDBacklight

//-----------------------------------
void turnOffLCDBacklight()
{
	//turn off LCD backlight
	//  by giving 5 volt to K of LCD
	lcd_Backlight_port |= _BV(lcd_Backlight_bit);
}//turnOffLCDBacklight

//----------------------------------
void LCD_blankChar(uint8_t num)
{
	uint8_t i;
	for (i=0; i<num; i++)
	lcd_write_character_4d(' ');
}//LCD_blankChar

//-----------------------------------
void lcd_SysTime()
{
	// set cursor to start of first line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	lcd_write_string_4d((uint8_t *)__TIME__);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)

	// set cursor to start of first line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	lcd_write_string_4d((uint8_t *)__DATE__);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)

}//lcd_SysTime

//-----------------------------------
void lcd_dispON()
{
	// set LCD off
	lcd_write_instruction_4d(lcd_DisplayOn);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
}//lcd_dispON

//-----------------------------------
void lcd_dispOFF()
{
	// set LCD off
	lcd_write_instruction_4d(lcd_DisplayOff);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
}//lcd_dispOFF

//-----------------------------------
void lcd_dispRealClock()
{
	char strSec[3], strMin[3], strHour[3];
	char strYear[3], strMonth[3], strDate[3];
	char strDay[10];
	// set cursor to start of first line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	// display the first line of information
	itoa(hour, strHour, 10);
	itoa(min, strMin, 10);
	itoa(sec, strSec, 10);
	lcd_write_string_4d((uint8_t *)strHour);
	lcd_write_string_4d((uint8_t *)":");
	lcd_write_string_4d((uint8_t *)strMin);
	lcd_write_string_4d((uint8_t *)":");
	lcd_write_string_4d((uint8_t *)strSec);
	lcd_write_string_4d((uint8_t *)"           ");
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)

	// set cursor to start of first line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	itoa(year, strYear, 10);
	itoa(month, strMonth, 10);
	itoa(date, strDate, 10);
	lcd_write_string_4d((uint8_t *)strYear);
	lcd_write_string_4d((uint8_t *)"/");
	lcd_write_string_4d((uint8_t *)strMonth);
	lcd_write_string_4d((uint8_t *)"/");
	lcd_write_string_4d((uint8_t *)strDate);
	lcd_write_string_4d((uint8_t *)" ");

	switch (day)
	{
		case 0:
		strcpy(strDay,"Sun");
		break;
		case 1:
		strcpy(strDay,"Mon");
		break;
		case 2:
		strcpy(strDay,"Tue");
		break;
		case 3:
		strcpy(strDay,"Wed");
		break;
		case 4:
		strcpy(strDay,"Thu");
		break;
		case 5:
		strcpy(strDay,"Fri");
		break;
		case 6:
		strcpy(strDay,"Sat");
		break;
	}
	lcd_write_string_4d((uint8_t *)strDay);
	lcd_write_string_4d((uint8_t *)"     ");
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)


}//lcd_dispRealClock

//-----------------------------------
void lcd_dispAccumulatedTime()
{
	char strSec[3], strMin[3], strHour[3];

	// set cursor to start of 2nd line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	// display the first line of information
	itoa(accumulatedHour, strHour, 10);
	itoa(accumulatedMin, strMin, 10);
	itoa(accumulatedSec, strSec, 10);
	lcd_write_string_4d((uint8_t *)strHour);
	lcd_write_string_4d((uint8_t *)":");
	lcd_write_string_4d((uint8_t *)strMin);
	lcd_write_string_4d((uint8_t *)":");
	lcd_write_string_4d((uint8_t *)strSec);
	lcd_write_string_4d((uint8_t *)"       ");
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
}//lcd_dispAccumulatedTime

//-----------------------------------
void lcd_showDHT11()
{
	int8_t temperature=0, humidity=0;
	char str[3];

	//dht_getdata(&temperature, &humidity);
	//dht_gettemperaturehumidity(&temperature, &humidity);

	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)

	lcd_write_string_4d((uint8_t *)"Temp:");

	//lcd_write_4(temperature);
	//sprintf(str, "%d", temperature);
	//temperature = 19;
	itoa(temperature,str, 10);
	lcd_write_string_4d((uint8_t *)str);
	lcd_write_string_4d((uint8_t *)"    ");
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)

	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	lcd_write_string_4d((uint8_t *)"Humidity:");

	itoa(humidity,str, 10);
	lcd_write_string_4d((uint8_t *)str);
	lcd_write_string_4d((uint8_t *)"        ");
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
}//lcd_showDHT11

//-----------------------------------
void lcd_dispProgInfo()
{
	// set cursor to start of first line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	// display the first line of information
	lcd_write_string_4d(program_author);

	// set cursor to start of second line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)

	// display the second line of information
	lcd_write_string_4d(program_version);

	_delay_ms(1000);

	// set cursor to start of second line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)

	// display the second line of information
	lcd_write_string_4d(program_date);

	_delay_ms(2000);

}//lcd_dispProgInfo

//-----------------------------------
void lcd_dispMenu()
{
	// set cursor to start of first line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	// display the first line of information
	lcd_write_string_4d(menu_str1);

	// set cursor to start of second line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)

	// display the second line of information
	lcd_write_string_4d(menu_str2);

	_delay_ms(3000);

}//lcd_dispMenu

//-----------------------------------
void lcd_dispWords(uint8_t i)
{
	uint8_t n, wordLen;
	uint8_t *words0=0, *words1=0;
	//uint8_t str[3];
	// set cursor to start of first line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	// display the first line of information
	//words = malloc(320);
	switch (min%4)
	{
		case 0:
		case 1:
		words0 = words000[i][0];
		words1 = words000[i][1];
		break;
		case 2:
		case 3:
		words0 = words001[i][0];;
		words1 = words001[i][1];
		break;
	}
	//lcd_write_string_4d(words000[i][0]);
	lcd_write_string_4d(words0);
	//wordLen = strlen((char *)words000[i][0]);
	wordLen = strlen((char *)words0);
	for (n=0; n<(LCD_MAXCOL-wordLen); n++)
	lcd_write_character_4d((uint8_t)0x20);
	//lcd_write_string_4d(program_author);

	// set cursor to start of second line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)

	// display the second line of information
	//lcd_write_string_4d(words000[i][1]);
	lcd_write_string_4d(words1);
	//utoa(sizeof(words000[0][0]), str, 10); //16
	//utoa(sizeof(words000[0]), str, 10); //32
	//utoa(sizeof(words), str, 10); //?
	//utoa(strlen(words000[i][0]), str, 10);
	//lcd_write_string_4d(str);
	//wordLen = strlen((char *)words000[i][1]);
	wordLen = strlen((char *)words1);
	for (n=0; n<(LCD_MAXCOL-wordLen); n++)
	lcd_write_character_4d((uint8_t)0x20);
	//lcd_write_string_4d(program_date);

	//_delay_ms(2000);

}//lcd_dispWords

//-----------------------------------
void lcd_testString()
{
	// set cursor to start of first line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	// display the first line of information
	lcd_write_string_4d(program_author);

	// set cursor to start of second line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)

	// display the second line of information
	lcd_write_string_4d(program_version);

	_delay_ms(1000);

	// set cursor to start of second line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	// display the first line of information
	lcd_write_string_4d(program_date);

	_delay_ms(1000);

	lcd_write_instruction_4d(lcd_DisplayOff);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	_delay_ms(1000);
	lcd_write_instruction_4d(lcd_DisplayOn);

}//lcd_testString

//-----------------------------------
void LCD_dispNotice()
{
	makePDasOutput();

	lcd_init_4d();
	_delay_us(1000);                                  // 40 uS delay (min)

	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	lcd_write_string_4d((uint8_t *)"Ready to run!");
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	lcd_write_string_4d((uint8_t *)"Now,sleep.Bye.");
	_delay_ms(3000);
	
	makePDasInput();
}//LCD_dispNotice

//-----------------------------------
void chkButtonAndToggleBacklight()
{
	uint8_t valSwitch;

	valSwitch = tactile_Switch_port & _BV(tactile_Switch_bit);
	_delay_ms(200);

	//if switch is pressed,
	if (valSwitch == 0)
	//toggle backlight by allow K to touch ground
	lcd_Backlight_port ^= _BV(lcd_Backlight_bit);
}//chkButtonAndToggleBacklight


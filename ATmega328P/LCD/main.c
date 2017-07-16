/****************************************
Target MCU & clock speed: ATmega328P @ 1Mhz internal
Name : main.c
	C modules of this project, LCD:
	main.c globals.c intrpt.c strFunc.c util.c
	custom Headers:
	defines.h externs.h
Author  : Insoo Kim (insoo@hotmail.com)
Created : May 15, 2015
Updated : Jul 12, 2017 (On Atmel Studio 7)
	Oct 9, 2016 (On Atmel Studio 7)

Description: 
	** Jul 12, 2017 (On Atmel Studio 7)
	Attach DS1307 RTC chip w/32Khz crystal. 
	Deeply power save by inventing LCD & DS1307 chip pin IO and Vcc, Vdd management.
	Sleep: 1.5 uA
	Run: 4 mA

	** Oct 9, 2016 (On Atmel Studio 7)
	Get system compile time & date and display on LCD 2*16
	Button toggling to turn on or off the backlight of LCD

HEX size[Byte]: 
	6504 w/ATmel Studio 7

Build & Upload to MCU in Windows Atmel Studio
	Jul 12, 2017 (On Atmel Studio 7)
	도구(T) - 외부도구(E)에서 아래처럼 추가
	제목: USBtin&Y
	명령: C:\WinAVR-20100110\bin\avrdude.exe
	인수: avrdude -c usbtiny -P usb -p atmega328p -U flash:w:LCD_RTC_DS1307_DHT11.hex:i
	초기 디렉토리$(ProjectDir)\Debug
	출력 창 사용 (체크)

	완료되면 도구(T) -  USBtinY(Y에 밑줄) 생성
	이제 USBtiny programmer를 PC에 연결하고, build(F7) 한 이후 Alt+T, Y하면 업로드 됨

	cd "C:\Users\insoo\Documents\GitHub\ATmelStudio\ATmega328P\LCD\Debug"

	Oct 9, 2016 (On Atmel Studio 7)
	cd "Atmel Studio\7.0\ATmega328P\LCD\LCD\Debug"
	OR cd "C:\Users\insoo\Documents\Atmel Studio\7.0\ATmega328P\LCD\LCD\Debug"
	OR cd "C:\Users\insoo\Box Sync\BoxElec\DIY Electronics\Arduino\Codes Atmel Studio\7.0\ATmega328P\LCD\LCD\Debug"
	avrdude -c usbtiny -P usb -p atmega328p -U flash:w:LCD.hex:i

 Ref:
    Donald Weiman    (weimandn@alfredstate.edu)
    http://web.alfredstate.edu/weimandn/programming/lcd/ATmega328/LCD_code_gcc_4d.html
 *******************************************/

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
int main()
{
    config();
 
    initINT();//to make English Words go round

	I2C_Init();
    parseCompileTime();
	
	//After call dispNotice(), sleep current remain rather high 680 uA
	//	otherwise, 230 uA
	//As of July 15, 2017, this function does not cause any further current draw than 0.2uA
	dispNotice(); 

    //lcd_dispRealClock();
	//lcd_dispProgInfo(); //LCD display program info
    // Use the Power Down sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	//BCD formt byte( wkDay, month, day, year,  ampm,  h,  m,  s)
	//setTime2DS1307(0x06, 0x07, 0x15, 0x17, PM, 0x01, 0x19, 0x30 );

    // endless loop
    while(1)
    {
      // go to sleep and wait for interrupt...
      sleep_mode();
	  
	  //------------test LCD DS1307
	  //ShowI2Cdevices();
	  //LCD_BCDDigits(0x37);
	  //LCD_WriteDate();
	  //LCD_TimeDate();
	  //_delay_ms(1000);

      //sysClockTest();
      //lcd_dispRealClock();
      //lcd_showDHT11();
      //_delay_ms(1000);
      //lcd_dispProgInfo();
      //_delay_ms(1000);

	  //testTimingOnDebugPin()
	      }
    return 0;
}//main

//-----------------------------------
void dispNotice()
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
}//dispNotice

//-----------------------------------
void testTimingOnDebugPin()
{
    debug_PIN_port |= _BV(debug_PIN_bit);
    _delay_ms(100);
    debug_PIN_port &= ~_BV(debug_PIN_bit);
    _delay_ms(100);
}//testTimingOnDebugPin

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


//-----------------------------------




// ---------------------------------------------------------------------------
// I2C (TWI) ROUTINES
//
// On the AVRmega series, PA4 is the data line (SDA) and PA5 is the clock (SCL
// The standard clock rate is 100 KHz, and set by I2C_Init. It depends on the AVR osc. freq.
/*
#define F_SCL 100000L // I2C clock speed 100 KHz
#define READ 1
#define TW_START 0xA4 // send start condition (TWINT,TWSTA,TWEN)
#define TW_STOP 0x94 // send stop condition (TWINT,TWSTO,TWEN)
#define TW_ACK 0xC4 // return ACK to slave
#define TW_NACK 0x84 // don't return ACK to slave
#define TW_SEND 0x84 // send data (TWINT,TWEN)
#define TW_READY (TWCR & 0x80) // ready when TWINT returns to logic 1.
#define TW_STATUS (TWSR & 0xF8) // returns value of status register
#define I2C_Stop() TWCR = TW_STOP // inline macro for stop condition
*/


// ---------------------------------------------------------------------------
// HD44780-LCD DRIVER ROUTINES
//
void LCD_Hex(int data)
// displays the hex value of DATA at current LCD cursor position
{
	char st[8] = ""; // save enough space for result
	itoa(data,st,16); // convert to ascii hex
	//LCD_Message("0x"); // add prefix "0x" if desired
	//LCD_String(st); // display it on LCD
	lcd_write_string_4d((uint8_t *)st);
}


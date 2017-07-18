/****************************************
Target MCU & clock speed: ATmega328P @ 1Mhz internal
Name : main.c
	C modules of this project, LCD:
	main.c globals.c intrpt.c strFunc.c util.c
	custom Headers:
	defines.h externs.h
Author  : Insoo Kim (insoo@hotmail.com)
Created : May 15, 2015
Updated : 
	Jul 16, 2017:(On Atmel Studio 7 and GitHub) 
	Dispatch intermixed functions into separte and corresponding code files.
	
	Jul 12, 2017 (On Atmel Studio 7)
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
	// configure MCU IO pins
	// implemented at "config.c"
    config();
 
	// setup ISR
	// implemented at "intrpt.c"
    initINT();

	// setup ISR
	// implemented at "I2C.c"
	I2C_Init();

	// Before adding RTC, i get pseudo-real-time from system compile time
	// and check it as the start time of the system.
	// This is a legacy function.
	// implemented at "strFunc.c"
    parseCompileTime();
	
	//After call LCD_dispReadyAndSleep(), sleep current remain rather high 680 uA
	//	otherwise, 230 uA
	//As of July 15, 2017, this function does not cause any further current draw than 0.2uA
	//LCD_dispReadyAndSleep(); 

    //lcd_dispRealClock();
	//lcd_dispProgInfo(); //LCD display program info
    // Use the Power Down sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	//BCD formt byte( wkDay, month, day, year,  ampm,  h,  m,  s)
	//setTime2DS1307(0x02, 0x07, 0x18, 0x17, AM, 0x09, 0x58, 0x30 );
	prepareWakeUpandLCDHome();
	getDHT();
	_delay_ms(500);
	getDHT();
	EW_tempHumid();
	//EW_Time();

	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	  ER_Byte_LCD(0);
	  ER_Byte_LCD(1);

    // endless loop
    while(1)
    {
      // go to sleep and wait for interrupt...
      sleep_mode();
	  
	}
    return 0;
}//main

//-----------------------------------
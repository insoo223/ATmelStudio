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
<<<<<<< HEAD
	Feb 13, 2019: Git cloned at E5200 PC and built successfully!
	
=======
>>>>>>> 638ff2a3eb2130966f947a85662ff9203998d75b
	Jan 24, 2019: Assign File Part Number as 23199817 
		and Boxed as ATmega328p-LCD_RTC_DS1307_DHT11-23199817(Jan24,2019).zip
		at BoxElec - DIY Electronics - Arduino - AVR C - 
			Win_Atmel Studio_Codes - 7.0 - ATmega328P -
			- LCD - LCD_RTC_DS1307_DHT11 - CodeSnapShots
	Jul 16, 2017:(On Atmel Studio 7 and GitHub) 
	Dispatch intermixed functions into separate and corresponding code files.
	
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
	Button toggling to turn on or off the back light of LCD

HEX size[Byte]: 
	6504 w/ATmel Studio 7

Build & Upload to MCU in Windows Atmel Studio
	Jul 12, 2017 (On Atmel Studio 7)
	도구(T) - 외부도구(E)에서 아래처럼 추가
	제목: USBtin&Y

<<<<<<< HEAD
	command (명령): 
	(2019.2.13) C:\Users\Insoo\.platformio\packages\tool-avrdude\avrdude.exe
	C:\WinAVR-20100110\bin\avrdude.exe
	
	arguments (인수): 
	(2019.2.13) -c usbtiny -P usb -p atmega328p -U flash:w:LCD_RTC_DS1307_DHT11.hex:i
	avrdude -c usbtiny -P usb -p atmega328p -U flash:w:LCD_RTC_DS1307_DHT11.hex:i
	
	ATtiny13용 프로젝트에선 아래 처럼.
	avrdude  -c usbtiny -P usb -p attiny13 -U flash:w:ISR-BathBulbAC1W.hex:i

	초기 디렉토리 $(ProjectDir)\Debug
=======
	명령: C:\WinAVR-20100110\bin\avrdude.exe

	인수: avrdude -c usbtiny -P usb -p atmega328p -U flash:w:LCD_RTC_DS1307_DHT11.hex:i
	ATtiny13용 프로젝트에선 아래 처럼.
	avrdude  -c usbtiny -P usb -p attiny13 -U flash:w:ISR-BathBulbAC1W.hex:i

	초기 디렉토리$(ProjectDir)\Debug
>>>>>>> 638ff2a3eb2130966f947a85662ff9203998d75b
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
	// This is a legacy function implemented at "strFunc.c"
    //parseCompileTime();
	
	//After call LCD_dispReadyAndSleep(), sleep current remain rather high 680 uA
	//	otherwise, 230 uA
	//As of July 15, 2017, this function does not cause any further current draw than 0.2uA
	//LCD_dispReadyAndSleep(); 

    //lcd_dispRealClock();
	//lcd_dispProgInfo(); //LCD display program info
    // Use the Power Down sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	//-- Set Current Time
	//BCD format byte( wkDay, month, day, year,  ampm,  h,  m,  s)
	//setTime2DS1307(0x02, 0x07, 0x18, 0x17, AM, 0x09, 0x58, 0x30 ); //Tue July 18, 2017 
	//Thu Jan 24, 2019 PM 9:22:20 (Even though, notify PM hour should be set as 24 hour mode
	//setTime2DS1307(0x04, 0x01, 0x24, 0x19, PM, 0x09, 0x28, 0x20 ); 
	//Tue Jan 29, 2019 AM 11:33:40 
	//setTime2DS1307(0x02, 0x01, 0x29, 0x19, AM, 0x11, 0x33, 0x40 );
	//Tue Jan 29, 2019 PM 16:19:10
	//setTime2DS1307(0x02, 0x01, 0x29, 0x19, PM, 0x16, 0x19, 0x10 );
	//Wed Jan 30, 2019 AM 11:51:20
	//setTime2DS1307(0x03, 0x01, 0x30, 0x19, AM, 0x11, 0x51, 0x10 );
	//Wed Jan 30, 2019 PM 18:21:10
	//setTime2DS1307(0x03, 0x01, 0x30, 0x19, PM, 0x18, 0x27, 0x30 );
	//Thu Jan 31, 2019 PM 17:20:10
	//setTime2DS1307(0x04, 0x01, 0x31, 0x19, PM, 0x17, 0x20, 0x10 );
	//Thu Feb 07, 2019 AM 10:42:40
	//setTime2DS1307(0x04, 0x02, 0x07, 0x19, PM, 0x13, 0x01, 0x20 );
	//Mon Feb 11, 2019 PM 10:21:30
	//setTime2DS1307(0x01, 0x02, 0x11, 0x19, PM, 0x10, 0x40, 0x30 );
	//Wed Feb 13, 2019 AM 9:14:30
	//setTime2DS1307(0x03, 0x02, 0x13, 0x19, AM, 0x9, 0x15, 0x10 );
<<<<<<< HEAD
	//Sat Aug 10, 2019 PM 2:50:00
	//setTime2DS1307(0x06, 0x08, 0x10, 0x19, PM, 0x15, 0x23, 0x00 );
	//Sun Aug 11, 2019 AM 7:52:00
	//setTime2DS1307(0x07, 0x08, 0x11, 0x19, AM, 0x07, 0x52, 0x00 );
=======
	//Sun Jun 16, 2019 AM 10:33:00
	//setTime2DS1307(0x07, 0x06, 0x16, 0x19, AM, 0x10, 0x34, 0x00 );
>>>>>>> 638ff2a3eb2130966f947a85662ff9203998d75b

	//EEPROM write & read test of DHT11 info
	// implemented at "EEPROM.c"
	//EWR_dht11();
	//EWR_ds1307();

	//-- Welcome message
	//powerOnWelcomeMessage();	
	//showPowerOnTime();
	//powerOnWelcomeMessage_Short();
	
	//DS1307_setSQW(); //enable 1hz square wave 

	//--- To use LCD while testing any of functions
	// make it commented when in production mode
	prepareWakeUpandLCDHome();
<<<<<<< HEAD
	buzzerBeepBeep();
=======

>>>>>>> 638ff2a3eb2130966f947a85662ff9203998d75b
	// endless loop
    while(1)
    {
      // go to sleep and wait for interrupt...
<<<<<<< HEAD
      sleep_mode();
	  
	  //LCD_WriteTimeDateTempHumid();
	  //_delay_ms(1000);
	  
=======
      //sleep_mode();
	  LCD_WriteTimeDateTempHumid();
	  _delay_ms(1000);
>>>>>>> 638ff2a3eb2130966f947a85662ff9203998d75b
	  /*
	  if (doneTimer == 1)
	  {
		cli();
		prepareWakeUpandLCDHome();
		LCD_WriteTimeDateTempHumid();
		_delay_ms(5000);
		makePDasInput();
		doneTimer = 0;
		sei();
	  }
	  */

	  //--- For Test
	  //buzzerBeepBeep();
	  //LCD_WriteTimeDateTempHumid();
	  //testCountSQW();
	  //_delay_ms(500);

	}
    return 0;
}//main

//-----------------------------------
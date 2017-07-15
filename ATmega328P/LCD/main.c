/****************************************************************************
<For Windows Atmel Studio>
cd "Atmel Studio\7.0\ATmega328P\LCD\LCD\Debug"
OR cd "C:\Users\insoo\Documents\Atmel Studio\7.0\ATmega328P\LCD\LCD\Debug"
OR cd "C:\Users\insoo\Box Sync\BoxElec\DIY Electronics\Arduino\Codes Atmel Studio\7.0\ATmega328P\LCD\LCD\Debug"
avrdude -c usbtiny -P usb -p atmega328p -U flash:w:LCD.hex:i

 */

/**************************************************************
 Target MCU & clock speed: ATmega328P @ 1Mhz internal
 Name    : main.c
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
 3340 out of 32K (all modules built together) w/prev. version of Atmel Studio
	5308 w/ATmel Studio 7

 Ref:
    Donald Weiman    (weimandn@alfredstate.edu)
    http://web.alfredstate.edu/weimandn/programming/lcd/ATmega328/LCD_code_gcc_4d.html
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


/******************************* Main Program Code *************************/
int main()
{
    config();
    //ioinit(); //dht11
    //init_devices();
 
    initINT();//to make English Words go round

	I2C_Init();
    parseCompileTime();
	
	//After call dispNotice(), sleep current remain rather high 680 uA
	//	otherwise, 230 uA
	//dispNotice(); 

    //lcd_dispRealClock();
	//lcd_dispProgInfo(); //LCD display program info
    // Use the Power Down sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	//setTime2DS1307();

    // endless loop
    while(1)
    {
      // go to sleep and wait for interrupt...
      sleep_mode();
	  
	  //------------test LCD DS1307
	  //ShowDevices();
	  //LCD_BCDDigits(0x37);
	  //WriteDate();
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
}
/******************************* End of Main Program Code ******************/
void setTime2DS1307()
{
	makePDasOutput();
	//turn on DS1307 power
	DS1307_VCC_port |= _BV(DS1307_VCC_bit);
	_delay_ms(10);
	SetTimeDate();
	_delay_ms(1000);
	
	LCD_TimeDate();
	_delay_ms(3000);
}//setTime2DS1307

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

void testTimingOnDebugPin()
{
    PORTB |= _BV(debug_PIN);
    _delay_ms(100);
    PORTB &= ~_BV(debug_PIN);
    _delay_ms(100);
}//testTimingOnDebugPin

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

void config()
{
// configure the microprocessor pins for the data lines
// 4 data lines - output
    lcd_D7_ddr |= _BV(lcd_D7_bit);
    lcd_D6_ddr |= _BV(lcd_D6_bit);
    lcd_D5_ddr |= _BV(lcd_D5_bit);
    lcd_D4_ddr |= _BV(lcd_D4_bit);

// LCD backlight cathode pin (K) - Output
    lcd_Backlight_ddr |= _BV(lcd_Backlight_bit);
    //turn off LCD backlight
	// for it is GND or cathode pin, make it HIGH turn it off
    lcd_Backlight_port |= _BV(lcd_Backlight_bit);

// LCD VSS pin (Power Supply) - Output
//added by Insoo (Jul 11, 2017)
	lcd_VDD_ddr |= _BV(lcd_VDD_bit);
	//turn off LCD power
	lcd_VDD_port &= ~_BV(lcd_VDD_bit); 

// DS1307 VCC pin (Power Supply) - Output
//added by Insoo (Jul 11, 2017)
	DS1307_VCC_ddr |= _BV(DS1307_VCC_bit);
	//turn off DS1307 RTC chip power
	DS1307_VCC_port &= ~_BV(DS1307_VCC_bit);


// DHT11 (temp & humid sensor) VCC pin (Power Supply) - Output
	//added by Insoo (Jul 14, 2017)
	DHT_VCC_ddr |= _BV(DHT_VCC_bit);
	//turn off DS1307 RTC chip power
	DHT_VCC_port &= ~_BV(DHT_VCC_bit);

//Tactile switch - Input
    tactile_Switch_ddr &= ~_BV(tactile_Switch_bit);

// configure the microprocessor pins for the control lines
// E line - output
    lcd_E_ddr |= _BV(lcd_E_bit);
// RS line - output
    lcd_RS_ddr |= _BV(lcd_RS_bit);

// debug pin - Output
    debug_PIN_ddr |= _BV(debug_PIN_bit);
    //turn off debug_PIN
    debug_PIN_port &= ~_BV(debug_PIN_bit);
    //turn on debug_PIN
    //debug_PIN_port |= _BV(debug_PIN_bit);

// initialize the LCD controller as determined by the defines (LCD instructions)
// initialize the LCD display for a 4-bit interface
    //lcd_init_4d();

}//config

void turnOnLCDBacklight()
{
    //turn on LCD backlight
    //  by giving 0 volt to K of LCD
    lcd_Backlight_port &= ~_BV(lcd_Backlight_bit);
}//turnOnLCDBacklight

void turnOffLCDBacklight()
{
    //turn off LCD backlight
    //  by giving 5 volt to K of LCD
    lcd_Backlight_port |= _BV(lcd_Backlight_bit);
}//turnOffLCDBacklight

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

void lcd_dispOFF()
{
// set LCD off
    lcd_write_instruction_4d(lcd_DisplayOff);
    _delay_us(DELAY_INST);                                  // 40 uS delay (min)
}//lcd_dispOFF

void lcd_dispON()
{
// set LCD off
    lcd_write_instruction_4d(lcd_DisplayOn);
    _delay_us(DELAY_INST);                                  // 40 uS delay (min)
}//lcd_dispON

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

void SetTimeDate()
// simple, hard-coded way to set the date.
{
	I2C_WriteRegister(DS1307,DAYOFWK_REGISTER, 0x06); //Wednesday
	_delay_ms(10);

	I2C_WriteRegister(DS1307,MONTHS_REGISTER, 0x07);
	//When F_CPU is 1Mhz , delay 10ms is required
	//as of July 11, 2017 (insoo tested and succeeded)
	_delay_ms(10); 
	I2C_WriteRegister(DS1307,DAYS_REGISTER, 0x15);
	_delay_ms(10);
	I2C_WriteRegister(DS1307,YEARS_REGISTER, 0x17);
	_delay_ms(10);
	I2C_WriteRegister(DS1307,HOURS_REGISTER, 0x11); // add 0x40 for PM
	//I2C_WriteRegister(DS1307,HOURS_REGISTER, 0x05+0x40); // add 0x40 for PM
	_delay_ms(10);
	I2C_WriteRegister(DS1307,MINUTES_REGISTER, 0x38);
	_delay_ms(10);
	I2C_WriteRegister(DS1307,SECONDS_REGISTER, 0x30);
	_delay_ms(10);
}

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

// ---------------------------------------------------------------------------
// APPLICATION ROUTINES
void ShowDevices()
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

void LCD_BCDDigits(uint8_t data)
// helper function for WriteDate()
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

void WriteDate()
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

}//WriteDate

void WriteTime()
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
}//WriteTime

void LCD_blankChar(uint8_t num)
{
	uint8_t i;
	for (i=0; i<num; i++)
		lcd_write_character_4d(' ');
}//LCD_blankChar

void LCD_TimeDate()
{
	//LCD_Line(0); 
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	WriteTime();
	LCD_blankChar(8);

	//LCD_Line(1); 
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	WriteDate();
	LCD_blankChar(8);
}//LCD_TimeDate

void LCD_TimeDateTempHumid()
{
	//LCD_Line(0);
	getDHT();
	//_delay_ms(1000);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	WriteTime();
	LCD_blankChar(1);
	lcd_write_string_4d((uint8_t *)strTemp);
	//LCD_blankChar(1);
	lcd_write_character_4d('C');

	//LCD_Line(1);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	WriteDate();
	LCD_blankChar(2);
	lcd_write_string_4d((uint8_t *)strHumid);
	//LCD_blankChar(1);
	//lcd_write_character_4d('%');
	lcd_write_character_4d(37);
}//LCD_TimeDateTempHumid

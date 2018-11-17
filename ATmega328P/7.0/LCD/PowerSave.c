/*************************************
 Target MCU & clock speed: ATmega328P @ 1Mhz internal
 Name    : PowerSave.c
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : 2017-07-16 오후 12:52:58
 Updated : 

 Description: Control MCU and pheriperal chip to enable deep power save.

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
void prepareWakeUpandLCDHome()
{
	makePDasOutput();
	lcd_init_4d();
	//lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	//_delay_us(DELAY_INST); // 40 uS delay (min)
	//lcd_write_string_4d((uint8_t *)"LCD ready!");
}//prepareWakeUpandLCDHome

//----------------------------------
//Return to normal usage mode
//after making them as input mode to remove power usage by LCD
//by calling makePDasInput(). (added by Insoo: Jul 12, 2017)
void makePDasOutput()
{
	// configure the microprocessor pins for the data lines
	// 4 data lines - output
	lcd_D7_ddr |= _BV(lcd_D7_bit);
	lcd_D6_ddr |= _BV(lcd_D6_bit);
	lcd_D5_ddr |= _BV(lcd_D5_bit);
	lcd_D4_ddr |= _BV(lcd_D4_bit);

	// LCD backlight cathode pin (K) - Output
	lcd_Backlight_ddr |= _BV(lcd_Backlight_bit);

	// LCD VSS pin (Power Supply) - Output
	lcd_VDD_ddr |= _BV(lcd_VDD_bit);
	//turn on LCD power
	lcd_VDD_port |= _BV(lcd_VDD_bit);
	_delay_ms(10);

	// DS1307 Vcc pin (Power Supply) - output
	DS1307_VCC_ddr |= _BV(DS1307_VCC_bit);
	//turn on DS1307 power
	DS1307_VCC_port |= _BV(DS1307_VCC_bit);
	_delay_ms(10);

	// DHT11 (temp & humid sensor) VCC pin (Power Supply) - Output
	//added by Insoo (Jul 14, 2017)
	DHT_VCC_ddr &= ~_BV(DHT_VCC_bit);
	//turn on DHT11 chip power
	DHT_VCC_port |= _BV(DHT_VCC_bit);


	//Tactile switch - Input
	//tactile_Switch_ddr |= _BV(tactile_Switch_bit);

	// configure the microprocessor pins for the control lines
	// E line - output
	lcd_E_ddr |= _BV(lcd_E_bit);
	// RS line - output
	lcd_RS_ddr |= _BV(lcd_RS_bit);
	// turn off RS
	lcd_RS_port &= ~_BV(lcd_RS_bit);

}//makePDasOutput

//----------------------------------
//to remove power usage by LCD (added by Insoo: Jul 12, 2017)
// Deep Power Save method in circuit
//As of July 15, 2017(Sat) pm12:30,
//	finally achieved sleep current of 0.2 uA or 200 nA
void makePDasInput()
{

	// turn off four data lines
	// following four lines added by Insoo on Jul 15, 2017
	lcd_D7_port &= ~_BV(lcd_D7_bit);
	lcd_D6_port &= ~_BV(lcd_D6_bit);
	lcd_D5_port &= ~_BV(lcd_D5_bit);
	lcd_D4_port &= ~_BV(lcd_D4_bit);

	// configure the microprocessor pins for the data lines
	// 4 data lines - output, but make it input
	lcd_D7_ddr &= ~_BV(lcd_D7_bit);
	lcd_D6_ddr &= ~_BV(lcd_D6_bit);
	lcd_D5_ddr &= ~_BV(lcd_D5_bit);
	lcd_D4_ddr &= ~_BV(lcd_D4_bit);

	// turn off backlight cathode(GND)
	// following one line added by Insoo on Jul 15, 2017
	lcd_Backlight_port = _BV(lcd_Backlight_bit);
	// LCD backlight cathode pin (K) - output, but make it input
	lcd_Backlight_ddr &= ~_BV(lcd_Backlight_bit);

	// turn off DS1307
	DS1307_VCC_port &= ~_BV(DS1307_VCC_bit);
	// then, DS1307 Vcc pin (Power Supply) - output, but make it input
	DS1307_VCC_ddr &= ~_BV(DS1307_VCC_bit);

	// DHT11 (temp & humid sensor) signal pin - Output/Input
	//turn off and
	DHT_SIG_port &= ~_BV(DHT_SIG_bit);
	//make it input
	DHT_SIG_ddr &= ~_BV(DHT_SIG_bit);
	DHT_SIG_port &= ~_BV(DHT_SIG_bit);

	//turn off DHT11 chip power
	DHT_VCC_port &= ~_BV(DHT_VCC_bit);
	//make DHT_VCC as input, added by Insoo (Jul 14, 2017)
	DHT_VCC_ddr &= ~_BV(DHT_VCC_bit);

	//Tactile switch - Input
	//tactile_Switch_ddr &= ~_BV(tactile_Switch_bit);

	// turn off Enable pin
	// following one line added by Insoo on Jul 15, 2017
	lcd_E_port &= ~_BV(lcd_E_bit);
	// configure the microprocessor pins for the control lines
	// E line - output, but make it input
	lcd_E_ddr &= ~_BV(lcd_E_bit);
	 
	// turn off RS
	lcd_RS_port &= ~_BV(lcd_RS_bit);
	// RS line - output, but make it input
	lcd_RS_ddr &= ~_BV(lcd_RS_bit);

	// turn off LCD power
	lcd_VDD_port &= ~_BV(lcd_VDD_bit);
	// then, LCD VSS pin (Power Supply) - output, but make it input
	lcd_VDD_ddr &= ~_BV(lcd_VDD_bit);

}//makePDasInput

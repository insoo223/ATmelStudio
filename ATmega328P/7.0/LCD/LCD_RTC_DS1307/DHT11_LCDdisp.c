/*
 * DHT11_OneLEDdisp.c
 *
 * Created: 2017-07-06 오전 7:57:23
 *  Author: insoo
 */ 

/* F_CPU should be 1Mhz for the DHT11 library working properly */
//#define F_CPU 1000000 
#define F_CPU 1000000L

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "externs.h"
#include "defines.h"

//#include "../Library/DHT11/tinudht/tinudht.h" //DHT11 library hader
#include "C:\Users\insoo\Box Sync\BoxElec\DIY Electronics\Arduino\AVR C\Win_Atmel Studio_Codes\7.0\ATtiny13A\Library\DHT11\tinudht\tinudht.h" //DHT11 library hader

#define TINUDHT_PIN PB1  


//Global variables

//function prototypes

//-------------------------------------------
//Interrupt Service Routines
//-----------------------------------

char strTemp[3], strHumid[3];

//-------------------------------------------
void getDHT()
{
	uint16_t temp; 
	uint16_t humid; 
	TinuDHT tinudht;

	tinudht_read(&tinudht, TINUDHT_PIN);
	temp = tinudht.temperature;
	humid = tinudht.humidity;

	// display the first line of information
	itoa(temp, strTemp, 10);
	itoa(humid, strHumid, 10);

	// set cursor to start of 1st line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	lcd_write_string_4d((uint8_t *)strTemp);

	// set cursor to start of 2nd line
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(DELAY_INST);                                  // 40 uS delay (min)
	lcd_write_string_4d((uint8_t *)strHumid);


}//getDHT



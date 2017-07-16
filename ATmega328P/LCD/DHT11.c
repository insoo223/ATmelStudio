/*************************************
Target MCU & clock speed: ATmega328P @ 1Mhz internal
Name    : DHT11.c
Author  : Insoo Kim (insoo@hotmail.com)
Created : 2017-07-06 오전 7:57:23
Updated : 

Description: 
	0) Use tinudhc library for ATmega328P and ATtiny13A.
	F_CPU should be 1Mhz for the DHT11 library working properly
	F_CPU is defined 1000000L at defines.h
	
	1) Functions of temp & humidity using DHT11 sensor.

HEX size[Byte]:

Ref:
https://tinusaur.org/2014/07/10/tinudht-attiny-library-for-dht11/
https://bitbucket.org/tinusaur/tinudht/downloads/
**************************************/

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "defines.h"
#include <util/delay.h>
#include <avr/sleep.h>
#include "externs.h"

//#include "../Library/DHT11/tinudht/tinudht.h" //DHT11 library hader
//#include "C:\Users\insoo\Box Sync\BoxElec\DIY Electronics\Arduino\AVR C\Win_Atmel Studio_Codes\7.0\ATtiny13A\Library\DHT11\tinudht\tinudht.h" //DHT11 library hader
#include "tinudht.h" //DHT11 library hader

char strTemp[3], strHumid[3];

//-------------------------------------------
void getDHT()
{
	uint16_t temp; 
	uint16_t humid; 
	TinuDHT tinudht;
	DHT_VCC_ddr |= _BV(DHT_VCC_bit);
	DHT_VCC_port |= _BV(DHT_VCC_bit);
	_delay_ms(500);
	tinudht_read(&tinudht, DHT_SIG_bit);
	_delay_ms(500);
	tinudht_read(&tinudht, DHT_SIG_bit);
	temp = tinudht.temperature;
	humid = tinudht.humidity;

	// display the first line of information
	itoa(temp, strTemp, 10);
	itoa(humid, strHumid, 10);

}//getDHT
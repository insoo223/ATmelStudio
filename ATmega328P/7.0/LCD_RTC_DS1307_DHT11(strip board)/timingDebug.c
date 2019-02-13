/**************************************
Target MCU & clock speed: ATmega328P @ 1Mhz internal
Name    : TimingDebug.c
Author  : Insoo Kim (insoo@hotmail.com)
Created : 2017-07-16 오후 12:37:18
	Dispatched from main.c file.

Updated : 
	
Description: 
	Check MCU clock speed via debug pin with oscilloscope.

HEX size[Byte]:

Ref:
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
 void testTimingOnDebugPin()
 {
	 debug_PIN_port |= _BV(debug_PIN_bit);
	 _delay_ms(100);
	 debug_PIN_port &= ~_BV(debug_PIN_bit);
	 _delay_ms(100);
 }//testTimingOnDebugPin

//-----------------------------------
void sysClockTest()
{
	debug_PIN_port |= _BV(debug_PIN_bit);
	_delay_ms(10);
	debug_PIN_port &= ~_BV(debug_PIN_bit);
	_delay_ms(10);

}//sysClockTest

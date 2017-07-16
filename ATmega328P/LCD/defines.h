/**************************************
 Target MCU & clock speed: ATmega328P @ 1Mhz internal
 Name    : defines.h
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : May 15, 2015
 Updated : 
	Jul 16, 2017 
	Dispatch sub-defines to explicit .h files, such as,
		definesIOpin.h, definesLCD.h, definesI2C.h  
	Put more structured comments

	May 16, 2015
	Get system compile time & date and display on LCD 2*16
	Button toggling to turn on or off the backlight of LCD

 Description: 

 HEX size[Byte]:

 Ref:
    Donald Weiman    (weimandn@alfredstate.edu)
    Summary:    4-bit data interface, busy flag not implemented.
		Any LCD pin can be connected to any available I/O port.
		Includes a simple write string routine.
    http://web.alfredstate.edu/weimandn/programming/lcd/ATmega328/LCD_code_gcc_4d.html
**************************************/
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
 
#include "definesDS1307.h"
#include "definesI2C.h"
=======

#include "definesDS1307.h"
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======

#include "definesDS1307.h"
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======

#include "definesDS1307.h"
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
#include "definesIOpin.h"
#include "definesLCD.h"

/*--- MCU CLOCK -------------------
datasheet: Ch.8.2.1 default clock source is 1Mhz
----------------------------------*/
#define F_CPU 1000000L

//for English vocabulary studiy application
#define MAXWORDCNT 10

//Multiple of _delay_ms(halfSec/4),
//  which is the unit delay of each while loop
#define menuSelectInterval 2

#define adjustTimeInterval 8
#define halfSec 500 // 0.5 second checked by oscilloscope

//--------------------------------------
// TYPEDEFS
typedef uint8_t byte; // I just like byte & sbyte better
typedef int8_t sbyte;

#define AM 0
#define PM 1



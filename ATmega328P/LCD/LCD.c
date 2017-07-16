/**************************************
 Target MCU & clock speed: ATmega328P @ 1Mhz internal
 Name    : LCD.c
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : May 15, 2015
 Updated : Jul 16, 2017
	
	2017-07-13 오후 9:23:40

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
 *************************************/


/*** Program Notes *******************

This program uses a 4-bit data interface but does not use the
busy flag to determine when the LCD controller is ready.  The
LCD RW line (pin 5) is not connected to the MCU and it must be
connected to GND for the program to function.

All time delays are longer than those specified in most datasheets
in order to accommodate slower than normal LCD modules.  This
requirement is well documented but almost always ignored.  The
information is in a note at the bottom of the right hand
(Execution Time) column of the instruction set.

************************************

The four data lines as well as the two control lines may be
    implemented on any available I/O pin of any port.  These are
    the connections used for this program:

+-----------+        +----------+
| ATmega328 |        |   LCD    |
|           |        |          |
|        PD7|------->|D7        |
|        PD6|------->|D6        |
|        PD5|------->|D5        |
|        PD4|------->|D4        |
|           |        |D3        |
|           |        |D2        |
|           |        |D1        |
|           |        |D0        |
|           |        |          |
|        PD3|------->|E         |
|           |  GND-->|RW        |
|        PD2|------->|RS        |
+-----------+        +----------+

************************************/


/*============================== 4-bit LCD Functions ======================*/

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

/*
  Name:     lcd_init_4d
  Purpose:  initialize the LCD module for a 4-bit data interface
  Entry:    equates (LCD instructions) set up for the desired operation
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/

void lcd_init_4d()
{
// Power-up delay
// initial 40 mSec delay
    //_delay_ms(100);
	_delay_ms(40); // insoo modifed (Jul 14, 2017)

// IMPORTANT - At this point the LCD module is in the 8-bit mode and it is expecting to receive
//   8 bits of data, one bit on each of its 8 data lines, each time the 'E' line is pulsed.
//
// Since the LCD module is wired for the 4-bit mode, only the upper four data lines are connected to
//   the microprocessor and the lower four data lines are typically left open.  Therefore, when
//   the 'E' line is pulsed, the LCD controller will read whatever data has been set up on the upper
//   four data lines and the lower four data lines will be high (due to internal pull-up circuitry).
//
// Fortunately the 'FunctionReset' instruction does not care about what is on the lower four bits so
//   this instruction can be sent on just the four available data lines and it will be interpreted
//   properly by the LCD controller.  The 'lcd_write_4' subroutine will accomplish this if the
//   control lines have previously been configured properly.

// Set up the RS and E lines for the 'lcd_write_4' subroutine.
    lcd_RS_port &= ~_BV(lcd_RS_bit);                // select the Instruction Register (RS low)
    lcd_E_port &= ~_BV(lcd_E_bit);                  // make sure E is initially low

// Reset the LCD controller
    lcd_write_4(lcd_FunctionReset);                 // first part of reset sequence
    //_delay_ms(10);                                  // 4.1 mS delay (min)
	_delay_ms(5); // insoo modifed (Jul 14, 2017)

    lcd_write_4(lcd_FunctionReset);                 // second part of reset sequence
    //_delay_us(200);                                 // 100uS delay (min)
	_delay_ms(120); // insoo modifed (Jul 14, 2017)

    lcd_write_4(lcd_FunctionReset);                 // third part of reset sequence
    //_delay_us(200);                                 // this delay is omitted in the data sheet
	_delay_ms(10); // insoo modifed (Jul 14, 2017)

// Preliminary Function Set instruction - used only to set the 4-bit mode.
// The number of lines or the font cannot be set at this time since the controller is still in the
//  8-bit mode, but the data transfer mode can be changed since this parameter is determined by one
//  of the upper four bits of the instruction.

    lcd_write_4(lcd_FunctionSet4bit);               // set 4-bit mode
    _delay_us(DELAY_INST);                                  // 40uS delay (min)

// Function Set instruction
    lcd_write_instruction_4d(lcd_FunctionSet4bit);   // set mode, lines, and font
    _delay_us(DELAY_INST);                                  // 40uS delay (min)

// The next three instructions are specified in the data sheet as part of the initialization routine,
//  so it is a good idea (but probably not necessary) to do them just as specified and then redo them
//  later if the application requires a different configuration.

// Display On/Off Control instruction
    lcd_write_instruction_4d(lcd_DisplayOff);        // turn display OFF
    _delay_us(DELAY_INST);                                  // 40uS delay (min)

// Clear Display instruction
    lcd_write_instruction_4d(lcd_Clear);             // clear display RAM
    _delay_ms(4);                                   // 1.64 mS delay (min)

// ; Entry Mode Set instruction
    lcd_write_instruction_4d(lcd_EntryMode);         // set desired shift characteristics
    _delay_us(DELAY_INST);                                  // 40uS delay (min)

// This is the end of the LCD controller initialization as specified in the data sheet, but the display
//  has been left in the OFF condition.  This is a good time to turn the display back ON.

// Display On/Off Control instruction
    lcd_write_instruction_4d(lcd_DisplayOn);         // turn the display ON
    _delay_us(DELAY_INST);                                  // 40uS delay (min)
}//lcd_init_4d

/*...........................................................................
  Name:     lcd_write_string_4d
; Purpose:  display a string of characters on the LCD
  Entry:    (theString) is the string to be displayed
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/
void lcd_write_string_4d(uint8_t theString[])
{
    volatile int i = 0;                             // character counter*/
    while (theString[i] != 0)
    {
        lcd_write_character_4d(theString[i]);
        i++;
        _delay_us(DELAY_INST);                              // 40 uS delay (min)
    }
}

/*...........................................................................
  Name:     lcd_write_character_4d
  Purpose:  send a byte of information to the LCD data register
  Entry:    (theData) is the information to be sent to the data register
  Exit:     no parameters
  Notes:    does not deal with RW (busy flag is not implemented)
*/

void lcd_write_character_4d(uint8_t theData)
{
    lcd_RS_port |= _BV(lcd_RS_bit);                 // select the Data Register (RS high)
    lcd_E_port &= ~_BV(lcd_E_bit);                  // make sure E is initially low
    lcd_write_4(theData);                           // write the upper 4-bits of the data
    lcd_write_4(theData << 4);                      // write the lower 4-bits of the data
}

/*...........................................................................
  Name:     lcd_write_instruction_4d
  Purpose:  send a byte of information to the LCD instruction register
  Entry:    (theInstruction) is the information to be sent to the instruction register
  Exit:     no parameters
  Notes:    does not deal with RW (busy flag is not implemented)
*/
void lcd_write_instruction_4d(uint8_t theInstruction)
{
    lcd_RS_port &= ~_BV(lcd_RS_bit);                // select the Instruction Register (RS low)
    lcd_E_port &= ~_BV(lcd_E_bit);                  // make sure E is initially low
    lcd_write_4(theInstruction);                    // write the upper 4-bits of the data
    lcd_write_4(theInstruction << 4);               // write the lower 4-bits of the data
}


/*...........................................................................
  Name:     lcd_write_4
  Purpose:  send a byte of information to the LCD module
  Entry:    (theByte) is the information to be sent to the desired LCD register
            RS is configured for the desired LCD register
            E is low
            RW is low
  Exit:     no parameters
  Notes:    use either time delays or the busy flag
*/
void lcd_write_4(uint8_t theByte)
{
    lcd_D7_port &= ~_BV(lcd_D7_bit);                        // assume that data is '0'
    if (theByte & 1<<7) lcd_D7_port |= _BV(lcd_D7_bit);     // make data = '1' if necessary

    lcd_D6_port &= ~_BV(lcd_D6_bit);                        // repeat for each data bit
    if (theByte & 1<<6) lcd_D6_port |= _BV(lcd_D6_bit);

    lcd_D5_port &= ~_BV(lcd_D5_bit);
    if (theByte & 1<<5) lcd_D5_port |= _BV(lcd_D5_bit);

    lcd_D4_port &= ~_BV(lcd_D4_bit);
    if (theByte & 1<<4) lcd_D4_port |= _BV(lcd_D4_bit);

// write the data
                                                    // 'Address set-up time' (40 nS)
    lcd_E_port |= _BV(lcd_E_bit);                   // Enable pin high
    _delay_us(1);                                   // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
    lcd_E_port &= ~_BV(lcd_E_bit);                  // Enable pin low
    _delay_us(1);                                   // implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)
}

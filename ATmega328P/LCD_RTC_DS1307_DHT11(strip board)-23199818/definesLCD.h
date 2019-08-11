/*
 * definesLCD.h
 *
 * Created: 2017-07-16 오전 11:01:09
 *  Author: insoo
 */ 
 
#ifndef DEFINESLCD_H_
#define DEFINESLCD_H_

//*** LCD HW condition
#define LCD_MAXCOL 16

/*** Instruction execution time delay
Execution time: 37 uS when OSC is 270 KHz,
with the exception of 1.52 ms for return home "lcd_Home"
***********************************/
//#define DELAY_INST 40
#define DELAY_INST 60 // insoo modifed (Jul 14, 2017)


/**** LCD COMMANDS (HD44780 chip) 
HD44780 datasheet on Box.com: 
https://app.box.com/s/4syf1cl50r4zbphnakaoo8rdskpgd7eq

Execution time: 37 uS when OSC is 270 KHz, 
	with the exception of 1.52 ms for return home "lcd_Home"
***********************************/
// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40                    // start of line 2
//#define   lcd_LineThree   0x14                  // start of line 3 (20x4)
//#define   lcd_lineFour    0x54                  // start of line 4 (20x4)
//#define   lcd_LineThree   0x10                  // start of line 3 (16x4)
//#define   lcd_lineFour    0x50                  // start of line 4 (16x4)

// LCD instructions
// datasheet: p24, p28-29

//Clears entire display and sets DDRAM address 0 in address counter.
#define lcd_Clear 0x00000001 

//Sets DDRAM address 0 in address counter. 
//Also returns display from being shifted to original position.
//DDRAM contents remain unchanged.
//In brief, return cursor to first position on first line
#define lcd_Home  0b00000010 

// Sets cursor move direction and specifies display shift.
// These operations are performed during data write and read.
// datasheet p28
// Data2	1	0
//	   1	I/D S
// shift cursor from left to right on read/write
#define lcd_EntryMode       0b00000110 

// datasheet p29 Table 7 (Cursor or display shift)
// Data4	3	2
//	   1	S/C R/L
//			0   0	cursor to left
//			0   1	cursor to right
//			1   0	disp to left
//			1   1	disp to right
// Nothing defined in the beginning about Cursor/Shift

// datasheet p28 Display on/off control
// Data3	2	1	0
//	   1	D	C	B  (Disp, Cursor, Cursor Blink)
// turn display off
#define lcd_DisplayOff      0b00001000 
// display on, cursor off, don't blink character
#define lcd_DisplayOn       0b00001100 

// datasheet p28 Function set
// Data5 4  3 2
//     1 DL N F	(Data Length, Lines, Font)	
#define lcd_FunctionReset   0b00110000 // reset the LCD

// datasheet p29 Table 8
// Data5 4  3 2
//     1 DL N F		line# Font	
//	        0 0		1	  5*8	
//	        0 1		1	  5*10
//	        1 0		2	  5*8 (Select this)
// 4-bit data, 2-line display, 5 x 7 font
#define lcd_FunctionSet4bit 0b00101000 

// datasheet p24
// Set DDRAM address
// DDRAM data is sent and received after this setting.
// Data7 6-0
//	   1 ADD
// set cursor position
#define lcd_SetCursor       0b10000000 


#endif /* DEFINESLCD_H_ */
/*
 * definesIOpin.h
 *
 Created: 2017-07-16 오전 10:55:47
 
 Author: insoo Kim (insoo@hotmail.com)
 
 Updated :
	Jan 28, 2019: Pin re-assigned for strip board implementation. Wired based on pins vicinity.

 */ 


#ifndef definesIOpin_H_
#define definesIOpin_H_

/**** MCU PIN assignment *******************
datasheet: Ch.
***********************************/

/*--- strip board PIN ASSIGNMENTS since Jan 28, 2019

ATmega328p	LCD		DS1307		DHT11	TACTILE_SW	BUZZER
PB0					VCC
PB1			K
PB2			D7
PB3			D6
PB4			D5
PB5			D4
PB6
PB7								DAT(SIG)

PD0													BUZ_PLUS
PD1			VDD
PD2					SQW
PD3			RS
PD4			E
PD5								VCC
PD6										SW1
PD7										SW2

PC0
PC1
PC2
PC3
PC4
PC5
PC6
--------------------------------------------------------*/

//------ LCD (Character 2 by 8) pin assignments 
// LCD interface (should agree with the diagram in LCD.c)
// make sure that the LCD RW pin is connected to GND

//added by Insoo (Jul 11, 2017)
#define lcd_VDD_port     PORTD      // lcd Vdd (Power supply) pin
#define lcd_VDD_bit      PORTD1 // From PORTB6 updated on Jan 28, 2019
#define lcd_VDD_ddr      DDRD

#define lcd_RS_port     PORTD       // lcd Register Select pin
#define lcd_RS_bit      PORTD3 // From 2 updated on Jan 28, 2019
#define lcd_RS_ddr      DDRD

#define lcd_E_port      PORTD                   // lcd Enable pin
#define lcd_E_bit       PORTD4 // From 3 updated on Jan 28, 2019
#define lcd_E_ddr       DDRD

#define lcd_D4_port     PORTB                   // lcd D4 connection
#define lcd_D4_bit      PORTB5 // From PORTD4 updated on Jan 28, 2019
#define lcd_D4_ddr      DDRB

#define lcd_D5_port     PORTB                   // lcd D5 connection
#define lcd_D5_bit      PORTB4 // From PORTD5 updated on Jan 28, 2019
#define lcd_D5_ddr      DDRB

#define lcd_D6_port     PORTB                   // lcd D6 connection
#define lcd_D6_bit      PORTB3 // From PORTD6 updated on Jan 28, 2019
#define lcd_D6_ddr      DDRB

#define lcd_D7_port     PORTB                   // lcd D7 connection
#define lcd_D7_bit      PORTB2 // From PORTD7 updated on Jan 28, 2019
#define lcd_D7_ddr      DDRB

#define lcd_Backlight_port PORTB
#define lcd_Backlight_bit  PORTB1 // From PORTB0 updated on Jan 28, 2019
									// LCD K pin, active low
#define lcd_Backlight_ddr  DDRB


//------ DS1307 Real-Time clock chip pin assignments
//added by Insoo (Jul 12, 2017)
#define DS1307_VCC_port     PORTB   // DS1307 VCC (Power supply) pin
#define DS1307_VCC_bit      PORTB0  // From PORTB7 updated on Jan 28, 2019
#define DS1307_VCC_ddr      DDRB

//added by Insoo (Jan 28, 2019)
#define DS1307_SQW_port     PIND   // DS1307 SQW (Square Wave or Control) pin
#define DS1307_SQW_bit      PORTD2  
#define DS1307_SQW_ddr      DDRD

//------ DHT11 digital temperature & humidity sensor pin assignments
//added by Insoo (Jul 14, 2017)
#define DHT_VCC_port     PORTD
#define DHT_VCC_bit      PORTD5  // From PORTB2 updated on Jan 28, 2019
#define DHT_VCC_ddr      DDRD

//naming conformity by Insoo (Jul 16, 2017)
#define DHT_SIG_port     PORTB
#define DHT_SIG_bit		 PORTB7 // From PORTB1 updated on Jan 28, 2019
#define DHT_SIG_ddr      DDRB

//------ Switches pin assignments
#define tactile_Switch_port PIND // input pin
#define tactile_Switch_bit  PORTD6  // From PORTB4 updated on Jan 28, 2019
#define tactile_Switch_ddr  DDRD

//added by Insoo (Jan 28, 2019)
#define tactile_Switch2_port PIND
#define tactile_Switch2_bit  PORTD7  
#define tactile_Switch2_ddr  DDRD

//------ Buzzer pin assignments
//added by Insoo (Jan 28, 2019)
#define BUZZER_port      PORTD
#define BUZZER_bit       PORTD0
#define BUZZER_ddr       DDRD

//added by Insoo (Jul 14, 2017)
#define debug_PIN_port     PORTB      // debug pin
#define debug_PIN_bit      PORTB6	// From 3 updated on Jan 28, 2019
#define debug_PIN_ddr      DDRB

#endif /* definesIOpin_H_ */
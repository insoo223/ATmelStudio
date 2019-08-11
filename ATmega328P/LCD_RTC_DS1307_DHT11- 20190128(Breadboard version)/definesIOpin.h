/*
 * definesIOpin.h
 *
 * Created: 2017-07-16 오전 10:55:47
 *  Author: insoo
 */ 


#ifndef definesIOpin_H_
#define definesIOpin_H_

/**** MCU PIN assignment *******************
datasheet: Ch.
***********************************/
/*-------------------
--- PORTD for LCD 
PD7	6	5	4	3	2	1	0
D7	D6	D5	D4	E	RS	NC	NC 
-------------------*/
// LCD interface (should agree with the diagram in LCD.c)
//   make sure that the LCD RW pin is connected to GND
#define lcd_D7_port     PORTB                   // lcd D7 connection
#define lcd_D7_bit      PORTB7 // From PORTD7 updated on Jan 28, 2019
#define lcd_D7_ddr      DDRB

#define lcd_D6_port     PORTB                   // lcd D6 connection
#define lcd_D6_bit      PORTB3 // From PORTD6 updated on Jan 28, 2019
#define lcd_D6_ddr      DDRB

#define lcd_D5_port     PORTB                   // lcd D5 connection
#define lcd_D5_bit      PORTB4 // From PORTD5 updated on Jan 28, 2019
#define lcd_D5_ddr      DDRB

#define lcd_D4_port     PORTB                   // lcd D4 connection
#define lcd_D4_bit      PORTB5 // From PORTD4 updated on Jan 28, 2019
#define lcd_D4_ddr      DDRB

#define lcd_E_port      PORTD                   // lcd Enable pin
#define lcd_E_bit       PORTD4 // From 3 updated on Jan 28, 2019 
#define lcd_E_ddr       DDRD

#define lcd_RS_port     PORTD       // lcd Register Select pin
#define lcd_RS_bit      PORTD3 // From 2 updated on Jan 28, 2019 
#define lcd_RS_ddr      DDRD

/*-------------------
--- PORTB 
PB7			6		5	4		3		 2		 1		 0
DS1307_Vcc	LCD_Vdd NC	Switch	debugPin DHT_Vcc DHT_Sig NC
-------------------*/

//added by Insoo (Jul 12, 2017)
#define DS1307_VCC_port     PORTB   // DS1307 VCC (Power supply) pin
#define DS1307_VCC_bit      PORTB7
#define DS1307_VCC_ddr      DDRB

//added by Insoo (Jul 11, 2017)
#define lcd_VDD_port     PORTB      // lcd Vdd (Power supply) pin
#define lcd_VDD_bit      PORTB6
#define lcd_VDD_ddr      DDRB

#define tactile_Switch_port PINB
#define tactile_Switch_bit  PORTB4
#define tactile_Switch_ddr  DDRB

//added by Insoo (Jul 14, 2017)
#define debug_PIN_port     PORTB      // debug pin
#define debug_PIN_bit      PORTB3
#define debug_PIN_ddr      DDRB

//added by Insoo (Jul 14, 2017)
#define DHT_VCC_port     PORTB      
#define DHT_VCC_bit      PORTB2
#define DHT_VCC_ddr      DDRB

//naming conformity by Insoo (Jul 16, 2017)
#define DHT_SIG_port     PORTB
#define DHT_SIG_bit		 PORTB1 //DHT11 signal
#define DHT_SIG_ddr      DDRB

#define lcd_Backlight_port PORTB
#define lcd_Backlight_bit  PORTB0
#define lcd_Backlight_ddr  DDRB

#endif /* definesIOpin_H_ */
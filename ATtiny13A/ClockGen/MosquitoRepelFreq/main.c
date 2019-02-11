/*
 * main.c of MosquitoRepelFreq Project
 * Target MCU: Attiny13A , running @ 9.6MHZ
 * Created: 2017-07-03 오전 9:29:08
 * Author: Insoo Kim ( insoo@hotmail.com )
	Used IDE: ATmel Studio 7.0
	Last Updated:
 * Purpose: To make real-time clock with minimal resource, meaning no use of external clock and RTC module.
 * Output: 
	  Clock pulse(Square wave) on pin 5 (PB0)
 * Program upload at cmd window
 
 cd C:\Users\insoo\Box Sync\BoxElec\DIY Electronics\Arduino\AVR C\Win_Atmel Studio_Codes\7.0\ATtiny13A\ClockGen\MosquitoRepelFreq\Debug  
 
 avrdude -c usbtiny -P usb -p attiny13 -U flash:w:MosquitoRepelFreq.hex:i 

 * Ref:
	  https://www.instructables.com/answers/How-to-get-an-ATtiny-to-run-an-accurate-clock/
*/ 


#include <avr/io.h>

//function prototypes
void init();
void mosquitoRepelentFreq();

//---------------------------------------
int main(void)
{
	init();
    while (1) 
    {
    }
}//main

//---------------------------------------
void init()
{
	// OSCCAL calibrates 8 mhz internal timer speed
	// value of 96 was used to get output of 153600 Khz at
	// pin 5 on an osciilscope for this particular project
	
	OSCCAL = 0x00; // 158 Khz
	//OSCCAL = 0x3F; // 247 Khz
	//OSCCAL = 0x7F; // 542 Khz
	
	//CLKPR = 0x80; // set system clock to 8mhz with no prescale
	CLKPR = 0x08; // set system clock to 8mhz with prescale 256
	CLKPR = 0x00; // these 2 CLKPR instructions have to be run together in order
	
	// to set clock to 8 Mhz
	DDRB |= (1<<PB0); //Set pin PB0 as output (pinMode(0, OUTPUT)
	DDRB &= ~(1<<PB1); //Set pin PB1 as output (pinMode(0, INPUT)
	DDRB &= ~(1<<PB2); //Set pin PB2 as output (pinMode(0, INPUT)
	DDRB &= ~(1<<PB3); //Set pin PB3 as output (pinMode(0, INPUT)
	
	/* The Timer/Counter (TCNT0) and Output Compare Registers (OCR0A and OCR0B) are 8-bit
	registers. Interrupt request (abbreviated to Int.Req. in the figure) signals are all visible in the
	Timer Interrupt Flag Register (TIFR0). All interrupts are individually masked with the Timer Interrupt
	Mask Register (TIMSK0).*/
	TCNT0 = 0; // initialize timer counter value to 0
	
	/*The counting sequence is determined by the setting of the WGM01 and WGM00 bits located in
	the Timer/Counter Control Register (TCCR0A) and the WGM02 bit located in the Timer/Counter
	Control Register B (TCCR0B). There are close connections between how the counter behaves
	(counts) and how waveforms are generated on the Output Compare output OC0A.*/

	/*The mode of operation, i.e., the behavior of the Timer/Counter and the Output Compare pins, is
	defined by the combination of the Waveform Generation mode (WGM0[2:0]) and Compare Output
	mode (COM0x[1:0]) bits. The Compare Output mode bits do not affect the counting
	sequence, while the Waveform Generation mode bits do. The COM0x[1:0] bits control whether
	the PWM output generated should be inverted or not (inverted or non-inverted PWM). For non-
	PWM modes the COM0x[1:0] bits control whether the output should be set, cleared, or toggled
	at a Compare Match*/
	TCCR0A = 0; // write 0 to timer 0 control registers
	
	TCCR0B = 0;

	/*In Clear Timer on Compare or CTC mode (WGM0[2:0] = 2), the OCR0A Register is used to
	manipulate the counter resolution. In CTC mode the counter is cleared to zero when the counter
	value (TCNT0) matches the OCR0A. The OCR0A defines the top value for the counter, hence
	also its resolution. This mode allows greater control of the Compare Match output frequency. It
	also simplifies the operation of counting external events.*/

	/*Bits 7:6 – COM0A[1:0]: Compare Match Output A Mode
	These bits control the Output Compare pin (OC0A) behavior. If one or both of the COM0A[1:0]
	bits are set, the OC0A output overrides the normal port functionality of the I/O pin it is connected
	to. However, note that the Data Direction Register (DDR) bit corresponding to the OC0A pin
	must be set in order to enable the output driver.
	When OC0A is connected to the pin, the function of the COM0A[1:0] bits depends on the
	WGM0[2:0] bit setting.
	Table 11-2 shows the COM0A[1:0] bit functionality when the WGM0[2:0] bits are set to a normal
	or CTC mode (non-PWM).*/
	TCCR0A |= (1 << COM0A0); //Timer0 in toggle mode (Table 11-2 of datasheet)
	
	/*Bits 1:0 – WGM0[1:0]: Waveform Generation Mode
	Combined with the WGM02 bit found in the TCCR0B Register, these bits control the counting
	sequence of the counter, the source for maximum (TOP) counter value, and what type of waveform
	generation to be used, see Table 11-8 on page 73. Modes of operation supported by the
	Timer/Counter unit are: Normal mode (counter), Clear Timer on Compare Match (CTC) mode,
	and two types of Pulse Width Modulation (PWM) modes (see “Modes of Operation” on page 64).*/
	TCCR0A |= (1 << WGM01); //Start timer 1 in CTC mode (Table 11-8 )

	/*Bits 2:0 – CS0[2:0]: Clock Select
	The three Clock Select bits select the clock source to be used by the Timer/Counter.*/
	//TCCR0B |= (1 << CS02) | (1 << CS00); // div1024, Prescaler table 11.9 (p74)
	TCCR0B |= (1 << CS00); // Prescaler table 11.9 

	mosquitoRepelentFreq();
}//setup

//---------------------------------------
void mosquitoRepelentFreq()
{
	/*p75. The Output Compare Register A contains an 8-bit value that is continuously compared with the
	counter value (TCNT0). A match can be used to generate an Output Compare interrupt, or to
	generate a waveform output on the OC0A pin.*/
	//OCR0A=106; //T = 200ms at PB0, measured by OSC
	OCR0A=16; //f = 31Khz at PB0, measured by OSC
}//mosquitoRepelentFreq

//---------------------------------------

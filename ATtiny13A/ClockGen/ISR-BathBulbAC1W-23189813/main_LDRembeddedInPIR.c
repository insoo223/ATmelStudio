/**************************************************************
 Target MCU & clock speed: ATtiny13A @ 1.2Mhz internal
 Name: main.c
	Project folder: ISR-Bathroom LED-23189812
 C modules of this project:
    Nothing
 Custom Headers:
    Nothing
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : Sep 26, 2018
 Updated : Sep 29, 2018 (On Atmel Studio 7)

 Description: 
	In night, turning on bathroom light is too bright to do one's need.
		So I've used two white LEDs of normal 5mm diameter 
		with 510 ohm resistors, and just enough to see toilet crown.
		Here I prepare HW using my first PCB, and code using PCINT & WDT isr.

	ATtiny13A controls LEDs based on the composite input of 
		PIR motion sensor and CDS-5 light sensor. 

	PIR sensor triggers PCINT3 on PB3, and if sufficiently dark 
		by reading CDS-5 thru ADC, then two LEDs will lit up 
		for defined period (2 minute as of Sep 29, 2018)

 HEX size[Byte]: 332 out of 1024

How to upload to the target MCU
<For Windows Atmel Studio>
Select Tool - USBtiny 
(USBtiny menu should be configured in the external tool menu)

<For CMD window or DOS prompt>
cd " C:\Users\insoo\Documents\GitHub\ATmelStudio\ATtiny13A\ClockGen\ISR-Bathroom LED-23189812\Debug "
avrdude -c usbtiny -P usb -p attiny13 -U flash:w:ISR-Bathroom LED.hex:i

 Ref: PIR time duration & sensitivity control
 http://qqtrading.com.my/pir-motion-sensor-module-hc-sr501
 Software reset
 https://www.microchip.com/webdoc/AVRLibcReferenceManual/FAQ_1faq_softreset.html
 https://www.avrfreaks.net/forum/software-reset-6
 https://www.avrfreaks.net/forum/software-reset-avr-gcc
 Delay
  ATtiny13 - using delay() function
  https://forum.arduino.cc/index.php?topic=500256.0
  Time limit on delay()?
  http://forum.arduino.cc/index.php?topic=115473.0
  AVR�� ADC Noise Reduction Mode
  http://microchipdeveloper.com/8avr:adcnoisereduce
  Wake ADC after interrupting sleep mode
  https://forum.arduino.cc/index.php?topic=197568.0
  Power saving techniques for microprocessors
  https://www.gammon.com.au/power
*****************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

// ATtiny13A port usage
/*
#define LED0 PB0 //Original
#define LED1 PB1 //Original
#define CDS5 PB2 //Original
#define PIR  PB3 //Original
#define PIR_INT  PCINT3
*/
#define LED PB3 
#define PIR PB1 
#define PIR_INT  PCINT1

// Watch Dog Timer period [sec]
//#define UNIT_DELAY_WDT 1 
//#define UNIT_DELAY_WDT 4 
#define UNIT_DELAY_WDT 8

// accumulated WDT period [EA] to last LED ON. 
// total duration of LED ON [sec] = UNIT_DELAY_WDT * SET_DELAY_UNIT
//#define SET_DELAY_UNIT 3
//#define SET_DELAY_UNIT 15 // 2min when UNIT_DELAY_WDT is 8
#define SET_DELAY_UNIT 8 // 1min 4sec when UNIT_DELAY_WDT is 8

uint8_t i;
uint8_t LEDstatus=0;

uint8_t WDTtick=0; 
uint8_t PCINTtick=0; 

//-------- function prototypes --------
void systemInit(void);
void WDTsetup(void);
void toggleLED(void);
void enableWDT(void);

//-------- interrupt service routines --------
ISR(WDT_vect)
{
	// Disable PCINT interrupt
	// pin change interrupt occurs every rising and falling change
	// so that 
	//GIMSK &= ~(1<<PCIE); 

	// --------- INCREASE WDT TICK COUNT ----------------------
	// increase WDTtick every UNIT_DELAY_WDT sec
	++WDTtick;
	//toggleLED0();
	//testADC();

	// On the 1st SET_DELAY_UNIT, reset WDTtick count
	//		and stop WDT.
	if ( (WDTtick >= SET_DELAY_UNIT) && (LEDstatus == 1) )
	{
		
		WDTtick = 0; // Reset WDT counter value
		WDTCR &= ~(1<<WDTIE); // Disable watchdog timer interrupts
		PORTB &= ~(1<<LED); // off LEDs
		LEDstatus = 0; // make LEDstatus updated as 0
		
		//GIMSK |= (1<<PCIE); // Enable PCINT interrupt
	}//if (WDTtick >= SET_DELAY_UNIT)
	
}//ISR(WDT_vect)

ISR(INT0_vect)
{
	enableWDT();
}
/*
ISR(PCINT0_vect)
{
	PCINTtick++;
	if (PCINTtick % 2 == 1)
		//toggleLED();
		enableWDT();

}//ISR(PCINT0_vect)
*/
int main(void)
{
    systemInit();
	WDTsetup();

	// Use the Power Down sleep mode
	//set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	
	// ADC noise reduction sleep mode
	//set_sleep_mode(SLEEP_MODE_ADC);

	// Use the Power idle sleep mode
	set_sleep_mode(SLEEP_MODE_IDLE);

	while (1) 
	{
		// go to sleep and wait for interrupt...
		// 33 uA as of Sep 27, 2018 when sleep
		sleep_mode();

	}//while (1) 
}//main

void enableWDT(void)
{

	//if LED is off
	if (LEDstatus == 0)
	{
		WDTtick = 0; // Reset WDT counter value
		WDTCR |= (1<<WDTIE); // Enable watchdog timer interrupts
		PORTB |= (1<<LED); // turn on LED
		LEDstatus = 1; // make LEDstatus updated as 1
	}//if (LEDstatus == 0)
}//enableWDT


void toggleLED(void)
{
	PORTB ^= (1<<LED);	
}//toggleLED

void systemInit(void)
{
	DDRB |= (1<<LED) ; //output for LED
	PORTB &= ~(1<<LED) ;
	DDRB &= ~(1<<PIR); //input for PIR motion sensor

	MCUCR |= (1<<ISC01) | (1<<ISC00);	// Trigger INT0 on rising edge (ds 47)
	GIMSK |= (1<<INT0); // enable INT0 (PB1) external interrupt

	//PCMSK |= (1<<PIR_INT);   // pin change mask: listen to PORTB, pin PB1
	//GIMSK |= (1<<PCIE); // enable PCINT interrupt
	sei();          // enable all interrupts
}//systemInit

void WDTsetup(void)
{
	// set timer prescaler to UNIT_DELAY_WDT seconds
	// datasheet p43.
	switch (UNIT_DELAY_WDT)
	{
		case 50:
		WDTCR |= (1<<WDP2) | (1<<WDP0); // 0.5s
		break;
		case 1:
		WDTCR |= (1<<WDP2) | (1<<WDP1); // 1s
		break;
		case 4:
		WDTCR |= (1<<WDP3); // 4s
		break;
		case 8:
		WDTCR |= (1<<WDP3) | (1<<WDP0); // 8s
		break;
		default:
		WDTCR |= (1<<WDP3) | (1<<WDP0); // 8s
	}

	// Enable watchdog timer interrupts
	//WDTCR |= (1<<WDTIE);
}//WDTsetup


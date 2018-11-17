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
  AVR¢ç ADC Noise Reduction Mode
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
#define LED0 PB0
#define LED1 PB1
#define CDS5 PB2
#define PIR  PB3
#define PIR_INT  PCINT3

// AVR software reset macro
#define soft_reset()        \
do                          \
{                           \
	wdt_enable(WDTO_15MS);  \
	for(;;)                 \
	{                       \
	}                       \
} while(0)

// Watch Dog Timer period [sec]
//#define UNIT_DELAY_WDT 1 
//#define UNIT_DELAY_WDT 4 
#define UNIT_DELAY_WDT 8

// accumulated WDT period [EA] to last LED ON. 
// total duration of LED ON [sec] = UNIT_DELAY_WDT * SET_DELAY_UNIT
//#define SET_DELAY_UNIT 3
#define SET_DELAY_UNIT 15

// turning LED ON threshold of ADC output of CDS-5 voltage dividor
#define CDS5_LIGHT_THRESHOLD 50 //0 to 254

uint8_t i;
int adc_result;
//uint8_t isrCount=0;
uint8_t LEDstatus=0;

uint8_t WDTtick=0; 

//-------- function prototypes --------
void adc_setup (void);
void systemInit(void);
void WDTsetup(void);
void toggleLED0(void);
void testADC(void);
void blinkLEDcnt(uint8_t );
void checkAmbientLight(void);

//-------- interrupt service routines --------
ISR(WDT_vect)
{
	// --------- INCREASE WDT TICK COUNT ----------------------
	// increase WDTtick every UNIT_DELAY_WDT sec
	++WDTtick;
	//toggleLED0();
	//testADC();

	// On the 1st SET_DELAY_UNIT (2 min), reset WDTtick count
	//		and stop WDT.
	if ( (WDTtick >= SET_DELAY_UNIT) && (LEDstatus == 1) )
	{
		
		WDTtick = 0; // Reset WDT counter value
		WDTCR &= ~(1<<WDTIE); // Disable watchdog timer interrupts
		PORTB &= ~(1<<LED0) & ~(1<<LED1); // off LEDs
		LEDstatus = 0; // make LEDstatus updated as 0
		//soft_reset();
	}//if (WDTtick >= SET_DELAY_UNIT)

}//ISR(WDT_vect)

ISR(PCINT0_vect)
{
	//GIMSK &= ~(1<<PCIE); // disable PCINT interrupt

	//isrCount++;
	//if ((isrCount % 2) == 1)
	//if ( ((isrCount % 2) == 1) || ((isrCount % 2) == 0) )
	{
		//toggleLED0();
		adc_setup();
		checkAmbientLight();
	//GIMSK |= (1<<PCIE); // enable PCINT interrupt
	}

}//ISR(PCINT0_vect)

int main(void)
{
    systemInit();
	WDTsetup();
	adc_setup();

	// Use the Power Down sleep mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	
	// ADC noise reduction sleep mode
	//set_sleep_mode(SLEEP_MODE_ADC);


	while (1) 
	{
		// go to sleep and wait for interrupt...
		// 33 uA as of Sep 27, 2018 when sleep
		sleep_mode();

	}//while (1) 
}//main

void checkAmbientLight(void)
{
	adc_result = ADCH;

	// Start the next conversion
	ADCSRA |= (1 << ADSC);

	//if LED is off
	if (LEDstatus == 0)
	{
		//and if ambient light is dark enough
		if (adc_result < CDS5_LIGHT_THRESHOLD)
		{
			WDTtick = 0; // Reset WDT counter value
			WDTCR |= (1<<WDTIE); // Enable watchdog timer interrupts
			PORTB |= (1<<LED0) | (1<<LED1); // turn on LEDs
			LEDstatus = 1; // make LEDstatus updated as 1
		}//if (adc_result < CDS5_LIGHT_THRESHOLD)
	}//if (LEDstatus == 0)
}//checkAmbientLight

void testADC(void)
{
	adc_result = ADCH;

	// Start the next conversion
	ADCSRA |= (1 << ADSC);

	if (adc_result < 20)
		blinkLEDcnt(1);
	else if (adc_result < 50)
		blinkLEDcnt(2);
	else if (adc_result < 100)
		blinkLEDcnt(3);
	else
		blinkLEDcnt(4);

}//testADC

//Not so perfect if using WDT isr with _delay_ms
//	for maybe there's only one timer circuit in ATtiny13a
void blinkLEDcnt(uint8_t num)
{
	uint8_t i;

	for(i=0; i<num; i++)
	{
		PORTB ^= (1<<LED0); // toggle on/off LED
		_delay_ms(150);
	}
	PORTB &= ~(1<<LED0); //off LED
	_delay_ms(500);
}//blinkLEDcnt

void toggleLED0(void)
{
	PORTB ^= (1<<LED0);	
}//toggleLED0

void systemInit(void)
{
	DDRB |= (1<<LED0) | (1<<LED1); //output for LEDs
	PORTB &= ~(1<<LED0) & ~(1<<LED1);
	DDRB &= ~(1<<PIR); //input for PIR motion sensor

	MCUCR &= ~(1<<ISC01) | ~(1<<ISC00);	// Trigger INT0 on rising edge
	PCMSK |= (1<<PIR_INT);   // pin change mask: listen to portb, pin PB3
	GIMSK |= (1<<PCIE); // enable PCINT interrupt
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

void adc_setup (void)
{
	// Set the ADC input to PB2/ADC1, left adjust result
	ADMUX |= (1 << MUX0) | (1 << ADLAR);

	// Set the prescaler to clock/128 & enable ADC
	// At 9.6 MHz this is 75 kHz.
	// See ATtiny13 datasheet, Table 14.4.
	// Also works fine for 1.2Mhz clock of ATtiny13a (2018.9.27)
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);

	// Start the first conversion
	ADCSRA |= (1 << ADSC);
}//adc_setup

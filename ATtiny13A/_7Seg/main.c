 /*
 * LM35_ADC_5by7perfbd(NotYetWorking).c
 *
 * Created: 2017-07-08 ¿ÀÈÄ 5:12:43
 *  Author: insoo
 */ 

//#define F_CPU 9600000 /*if not, there's warning. The clock is set by IDE, i.e. ATmel Studio 7*/
#define F_CPU 1000000 /*if not, there's warning. The clock is set by IDE, i.e. ATmel Studio 7*/

#define REFVTG 5.0 //reference voltage for ADC

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define LED_PIN		PB0  // for 5*7 perf b'd
#define BUTTON_PIN	PB1  // for 5*7 perf b'd
#define LM35_PIN	PB4  // for 5*7 perf b'd

#define DURATION_BLINKLED 200 // [ms]

//Global variables

//function prototypes
void blinkLED(uint8_t );
void tuningCLK();
void initIO();
void initADC();
void setADC_ClockSpeed();
int adc_read ();
void chkTempLM35();

//-------------------------------------------
//Interrupt Service Routines
//-----------------------------------


//-------------------------------------------
int main(void)
{

	tuningCLK();
	initIO();
	initADC();
	setADC_ClockSpeed();


    for (;;) 
	{
		
		//blinkLED(resultADC/10);
		//_delay_ms(500);
		chkTempLM35();

    }//for

	return 0; // never reach here
}//main

//-----------------------------------
void blinkLED(uint8_t num)
{
	uint8_t i;
	for (i=0; i<(2*num); i++)
	{
		//digitalWrite(ledPin, HIGH);
		PORTB ^= _BV(LED_PIN);
		_delay_ms(DURATION_BLINKLED);
	}
}//blinkLED

void tuningCLK()
{
	//internal oscillator tuning
	OSCCAL -= 28; //calibrate internal OSC as of morning July 4, 2017

	//CLKPR = _BV(CLKPCE) | _BV(CLKPS3);
	//CLKPR = 0x00;
	//CLKPR = 0x88; // DIV256 system clock
	//CLKPR = 0x00; // these 2 CLKPR instructions have to be run together in order
}//tuningCLK

void initIO()
{
	//pin IO setting
	DDRB |= _BV(LED_PIN); // make LED_PIN (PB0, pin5) an output
	DDRB &= ~_BV(BUTTON_PIN); // make BUTTON_PIN (PB1, pin6) an input
	DDRB &= ~_BV(LM35_PIN); // make LM35_PIN (PB4, pin2) an input
}//initIO

void initADC()
{
	//ADC setting
	//Enable ADC pin input from LM35_PIN (PB3 or ADC3)
	//ADMUX |= _BV(MUX1) | _BV(MUX0);
	//Enable ADC pin input from LM35_PIN (PB4 or ADC2)
	ADMUX |= _BV(MUX1);
	//ADC Left Adjust Result
	//ADMUX |= _BV(ADLAR);  

}//initADC

void setADC_ClockSpeed()
{
	//Set the prescaler to clock/128 & enable ADC
	ADCSRA |= _BV(ADPS1) | _BV(ADPS0);

	// enable ADC
	ADCSRA |= _BV(ADEN);

}//setADC_ClockSpeed

int adc_read (void)
{
	//uint8_t dummy;
	// Start the conversion
	ADCSRA |= (1 << ADSC);

	// Wait for it to finish - blocking
	while (ADCSRA & (1 << ADSC));

	//return ADCH;
	//dummy = ADCH;
	return ADCL;
}//adc_read


void chkTempLM35()
{
	uint8_t  rawV;
	uint8_t temp, temp10, temp1;
	//uint8_t temp100;
	//uint8_t mV;
	//uint8_t K;
	//uint8_t C;
	  
	rawV = adc_read ();
	//mV = (rawV) * REFVTG /1.023;
	//K = (mV/10);
	//C= K - 273;
	//C = mV;
	
	//C = (rawV) * REFVTG; //to debug

	temp = (rawV >> 1) ; // arbitrary manipulation
	//temp += 7; // arbitrary manipulation
	//temp100 = temp/100;
	temp10 = temp / 10;
	temp1 = temp % 10;
	//blinkLED(temp100);
	//_delay_ms(500);
	blinkLED(temp10);
	_delay_ms(500);
	blinkLED(temp1);
	_delay_ms(1000);

}

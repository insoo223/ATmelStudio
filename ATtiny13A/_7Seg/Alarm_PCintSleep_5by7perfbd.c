/*
 * Alarm_PCintSleep_5by7perfbd.c
 *
 * Created: 2017-07-04 오전 11:05:54
 * Author : insoo
 */ 

#define F_CPU 1000000UL /*if not, there's warning. The clock is set by IDE, i.e. ATmel Studio 7*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define LED_PIN		PB0  // for 5*7 perf b'd
#define BUTTON_PIN	PB1  // for 5*7 perf b'd
#define SPK_PIN		PB2  // for 5*7 perf b'd

#define DURATION_BLINKLED 200 // [ms]
#define BUTTONCNT_TIMELIMIT 2 // [s]

//Global variables
uint8_t alarmPeriod[3] = {3,10,30}; // 3m9s, 10m30s, 31m30s

//function prototypes
void blinkLED(uint8_t );
void tuningCLK();
void initIO();
void initINTR();
void initINTR_PinChange();
void initINTR_TMR();
void disableINTR_TMR();
void chkButtonCnt();

//-------------------------------------------
//Interrupt Service Routines
//-----------------------------------

volatile uint8_t done = 0;

volatile unsigned char timer_overflow_count = 0;
uint8_t secInt=0, minInt=0;
uint8_t intDrivenAlmEn=0, intDrivenAlmPeriod = 10;
volatile unsigned char  cntButton, lapseTime, lapseStart;

ISR(PCINT0_vect)
{
	if (++done % 2 == 0)
	{
		//PORTB ^= _BV(LED_PIN);
		timer_overflow_count = 0;
		secInt = 0;
		minInt = 0;
		intDrivenAlmEn=1;
		lapseStart = 0;
		lapseTime = 0;

		cli();
		initINTR_TMR();
		//sleep_disable(); // 1000 uA
		set_sleep_mode(SLEEP_MODE_IDLE); // 420 uA
		sei();
		chkButtonCnt();
	}
	_delay_ms(200); //debounce

}//ISR(PCINT0_vect)

//-----------------------------------
ISR(TIM0_OVF_vect)
{
	//if (++timer_overflow_count > 60)
	if (++timer_overflow_count > 3)
	//if (++timer_overflow_count > 1)
	{   // with 1024/256/64 prescaler, a timer overflow occurs 4.6/18/73 times per second accordingly.
		// Toggle Port B pin 4 output state
		//PORTB ^= _BV(LED_PIN);
		timer_overflow_count = 0;
		secInt++;
		if (secInt == 60)
		{
			minInt++;
			secInt=0;
		}
		if (intDrivenAlmEn == 1)
			//if(minInt == intDrivenAlmPeriod)
			//if(secInt == intDrivenAlmPeriod)
			//if(secInt == alarmPeriod[(cntButton-1)%3])
			if(minInt == alarmPeriod[(cntButton-1)%3])
			{
				//buzz(shortBuzz);
				blinkLED(3);
				cntButton=0;
				lapseStart=0;
				lapseTime=0;
				secInt=0;
				minInt=0; 
				intDrivenAlmEn=0;

				cli();
				disableINTR_TMR();
				//sleep_enable();
				set_sleep_mode(SLEEP_MODE_PWR_DOWN); // 60 uA 
				sei();
			}
	}
}//ISR(TIM0_OVF_vect)

//-------------------------------------------
int main(void)
{
	tuningCLK();
	initIO();
	initINTR_PinChange();
	//initINTR_TMR();

	sei();

	// Use the Power Down sleep mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//set_sleep_mode(SLEEP_MODE_ADC);
	//set_sleep_mode(SLEEP_MODE_IDLE);

	sleep_enable();

    for (;;) 
	{
        // go to sleep and wait for interrupt...
        //sleep_mode();
		sleep_cpu();
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
	//BLINK_NOTICED = 1;
	PORTB &= ~_BV(LED_PIN);
	_delay_ms(DURATION_BLINKLED);
}//blinkLED

void tuningCLK()
{
	//internal oscillator tuning
	OSCCAL -= 28; //calibrate internal OSC as of morning July 4, 2017

	//CLKPR = _BV(CLKPCE) | _BV(CLKPS3);
	//CLKPR = 0x00;
	CLKPR = 0x88; // DIV256 system clock
	CLKPR = 0x00; // these 2 CLKPR instructions have to be run together in order
}//tuningCLK

void initIO()
{
	//pin IO setting
	DDRB |= _BV(LED_PIN); // make LED_PIN (PB0, pin5) an output
	DDRB &= ~_BV(BUTTON_PIN); // make BUTTON_PIN (PB1, pin6) an input
}//initIO

void initINTR_PinChange()
{
	//PC interrupt setting
	// Enable pin change interrupt for BUTTON_PIN
	PCMSK |= _BV(PCINT1);
	// enable PC(Pin Change) interrupt
	GIMSK |= _BV(PCIE);  //Enable PC interrupt

}//initINTR_PinChange

void initINTR_TMR()
{
	//TMR0 interrupt setting
	// prescale timer to 1/64th the clock rate
	//TCCR0B |= (1<<CS01) | (1<<CS00);

	// prescale timer to 1/1024th the clock rate
	TCCR0B |= (1<<CS02) | (1<<CS00);

	// enable timer overflow interrupt
	TIMSK0 |=1<<TOIE0;

}//initINTR_TMR

void disableINTR_TMR()
{
	//TMR0 interrupt setting
	// prescale timer to 1/64th the clock rate
	//TCCR0B |= (1<<CS01) | (1<<CS00);

	// prescale timer to 1/1024th the clock rate
	//TCCR0B |= (1<<CS02) | (1<<CS00);

	// enable timer overflow interrupt
	//TIMSK0 |=1<<TOIE0;
	// disable timer overflow interrupt
	TIMSK0 &= ~_BV(TOIE0);

}//disableINTR_TMR

void chkButtonCnt()
{
	lapseTime = secInt - lapseStart;
	if (lapseTime <= BUTTONCNT_TIMELIMIT)
	{
		cntButton++;
		lapseStart = secInt;
	}

}//chkButtonCnt
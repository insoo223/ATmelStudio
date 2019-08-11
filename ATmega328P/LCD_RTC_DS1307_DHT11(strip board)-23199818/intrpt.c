/*************************************
 Target MCU & clock speed: ATmega328P @ 1Mhz internal
 Name    : intrpt.c
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : May 15, 2015
 Updated : May 17, 2015

 Description: Interrupd Service Routines are implemented
	

 HEX size[Byte]:

 Ref:
    Donald Weiman    (weimandn@alfredstate.edu)
    http://web.alfredstate.edu/weimandn/programming/lcd/ATmega328/LCD_code_gcc_4d.html
 *********************************/

<<<<<<< HEAD
#include <stdio.h>
=======
  #include <stdio.h>
>>>>>>> 638ff2a3eb2130966f947a85662ff9203998d75b
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include "externs.h"
#include "defines.h"
#include <util/delay.h>

uint8_t pwrOn = 0;
volatile uint8_t btcnt = 0;
//----------------------------------
//Ref: http://www.avr-tutorials.com/interrupts/avr-external-interrupt-c-programming
//added by Insoo (Jan 30, 2019)
ISR(INT0_vect)
{
	numSQW++;
	if (numSQW == 3)
	{
		doneTimer = 1;

		buzzerBeepBeep();
		/*
		prepareWakeUpandLCDHome();
		LCD_WriteTimeDateTempHumid();
		_delay_ms(5000);
		makePDasInput();
		*/
		numSQW = 0;
		//EIMSK &= ~_BV(INT0); // Disable INT0 
	}
	
}
//ISR(PCINT0_vect) // test breadboard setup before Jan 28, 2019
ISR(PCINT2_vect)
{
    if ((btcnt++ % 2) == 0)
	{
		if (pwrOn == 0)
		{
			makePDasOutput();
			//make VDD pin as OUTPUT
			//lcd_VDD_ddr |= _BV(lcd_VDD_bit); //done by makePDasOutput();


			//_delay_ms(10);
			lcd_init_4d();
			//lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
			//lcd_write_string_4d((uint8_t *)"Init OK!");
			_delay_ms(10);

			pwrOn = 1;
		}//if (pwrOn == 0)
		//if (PINB & _BV(PB6))
		// read switch1 status
		if (tactile_Switch_port & _BV(tactile_Switch_bit))
		{
			/**********************************************/
			//Disable PC(Pin Change) interrupt
			//  to use the button for user menu selection
			//For PCINT7-0, DS: Ch 12.2.8
			//PCMSK0 &= ~_BV(PCINT6);
        
			//PCMSK0 &= ~_BV(tactile_Switch_bit);
			//_delay_ms(5);
			//check only rising or falling edge
			countButton();


			//Enable PC(Pin Change) interrupt
			//For PCINT7-0, DS: Ch 12.2.8
			//PCMSK0 |= _BV(PCINT6);
        
			//PCMSK0 |= _BV(tactile_Switch_bit);
			//_delay_ms(5);
			/***********************************************/

		}//if (tactile_Switch_port & _BV(tactile_Switch_bit))

		// read switch2 status
		/*
		if (tactile_Switch2_port & _BV(tactile_Switch2_bit))
		{
			//EIMSK = _BV(INT0); // Enable INT0 
			countButton();
		}
		*/
		_delay_ms(200); // for debounce
	}//if ((btcnt++ % 2) == 0)
}//ISR(PCINT2_vect)

//-----------------------------------
ISR(WDT_vect)
{
    //PORTB |= _BV(PB4);
<<<<<<< HEAD
    //proceedClock();
    //lcd_dispWords(wd%MAXWORDCNT);
    //wd++;
    //lcd_dispRealClock();
    //PORTB &= ~_BV(PB4);
	LCD_WriteTimeDateTempHumid();
=======
    proceedClock();
    lcd_dispWords(wd%MAXWORDCNT);
    wd++;
    //lcd_dispRealClock();
    //PORTB &= ~_BV(PB4);
>>>>>>> 638ff2a3eb2130966f947a85662ff9203998d75b
}//ISR(WDT_vect)

//-----------------------------------
void WDT_Init(void)
{
    //disable interrupts
    cli();
    //MCUSR = 0;

    wdt_disable();
    //set up WDT interrupt
    //WDTCSR = _BV(WDCE)|_BV(WDE);
    //Start watchdog timer with 4s prescaller
    //WDTCSR |= _BV(WDIE)|_BV(WDE)|_BV(WDP3);
    //WDTCSR |= _BV(WDIE)| _BV(WDP3);
    //WDTCSR |= _BV(WDIE)|_BV(WDE)|_BV(WDP2) | _BV(WDP1); // 1s

    //reset watchdog
    wdt_reset();
<<<<<<< HEAD
    //wdt_enable(WDTO_500MS); 
	wdt_enable(WDTO_8S); 
=======
    wdt_enable(WDTO_500MS);
>>>>>>> 638ff2a3eb2130966f947a85662ff9203998d75b
    //Enable global interrupts
    sei();
}//WDT_Init

//----------------------------------
void initINT()
{
    cli();
    //**** PC(Pin Change) interrupt setting
    //ref) https://gist.github.com/Wollw/2598827
    // enable PC INT
    //GIMSK |= _BV(PCIE);  //Enable PC interrupt
    // Enable pin change interrupt for PB3
    //PCMSK |= _BV(PCINT3);
    //PCMSK |= _BV(startPin);
	/**
	 * Pin Change Interrupt enable on PCINT6 (PB6)
	 */
    //For PCINT7-0, DS: Ch 12.2.4
	//PCICR |= _BV(PCIE0);
	//For PCINT7-0, DS: Ch 12.2.8
	//PCMSK0 |= _BV(tactile_Switch_bit);
    
	/**
	 * Pin Change Interrupt enable on PCINT22 (PD6)
	 * Added by Insoo (Jan 29, 2019)
	 */
	//For PCINT23-16, DS: Ch 12.2.4
    PCICR |= _BV(PCIE2); // enable Pin Change Intr from PCINT23 - PCINT16
	//PCMSK0 |= _BV(tactile_Switch_bit);
    // Enable pin change interrupt for PD6 & PD7
    PCMSK2 |= _BV(PCINT22);
	//PCMSK2 |= _BV(tactile_Switch_bit); 
	//PCMSK2 |= _BV(tactile_Switch_bit) | _BV(tactile_Switch2_bit);

	/**
	 * External Interrupt enable on INT0 (PD2)
	 * Added by Insoo (Jan 30, 2019)
	 * 
	 */
	//EICRA = _BV(ISC00) | _BV(ISC01);	// Let INT0 occurred at rising edge
	//EIMSK = _BV(INT0); // Enable INT0 

    //**** WDT interrupt setting
	//Following two sentences are in need, 
	// to make when WDT occurs, English Word round-robin display
    //check_wdt(); 
<<<<<<< HEAD
    setup_wdt(); 
=======
    //setup_wdt(); 
>>>>>>> 638ff2a3eb2130966f947a85662ff9203998d75b

    //set prescale timer
    //DS: ch10.9.2 table10.2
    //set up WDT interrupt
    //WDTCSR |= _BV(WDCE)| _BV(WDE);
    //WDTCSR = _BV(WDIE) | _BV(WDP3) | _BV(WDP0); // 8s
    //WDTCSR = _BV(WDIE) | _BV(WDP3); // 4s
    //WDTCSR = _BV(WDIE) | _BV(WDP2) | _BV(WDP0); // 0.5s


    //wdt_reset();
    //enalbe global interrupt
    sei();


}//initINT

//----------------------------------
void setup_wdt(void){
// Set up Watch Dog Timer for Inactivity
    // Enable the WD Change Bit
    // Enable WDT interrupt
    WDTCSR |= _BV(WDCE) | _BV(WDE);

    // Set Timeout to ~8 seconds
    WDTCSR = _BV(WDIE) | _BV(WDP3) | _BV(WDP0); // 8s
    // Set Timeout to ~1 seconds
    //WDTCSR =   _BV(WDIE) |  _BV(WDP2) | _BV(WDP1);
    // Set Timeout to ~500 ms
    //WDTCSR =   _BV(WDIE) | _BV(WDP2);
}//setup_wdt

//----------------------------------
/*
Utmost(!) help to get the WDT of ATmega328p work
http://elegantcircuits.com/2014/10/14/introduction-to-the-avr-watchdog-timer/
*/
void check_wdt(void)
{
	// If a reset was caused by the Watchdog Timer...
	if(MCUSR & _BV(WDRF))
	{
		// Clear the WDT reset flag
		MCUSR &= ~_BV(WDRF);
		// Enable the WD Change Bit
		WDTCSR |= (_BV(WDCE) | _BV(WDE));
		// Disable the WDT
		WDTCSR = 0x00;
	}
}//check_wdt

//----------------------------------
void init_devices(void){
    //stop errant interrupts until set up
    cli(); //disable all interrupts

    //timer0_init();

    MCUCR = 0x00;
    EICRA = 0x00; //extended ext ints
    EIMSK = 0x00;

    TIMSK0 = 0x02; //timer 0 interrupt sources

    PRR = 0x00; //power controller
    sei(); //re-enable interrupts
    //all peripherals are now initialized
}

//----------------------------------
void proceedClock()
{
    //WDT interrupt occurs every 8 seconds
    sec += 8;
    accumulatedSec += 8;

    //real-time fetched from compiled time constant __TIME__
    //calculate minutes
    if (sec >= 60)
    {
        sec%=60;
        min++;
        sec += 2;
    }
    //system run-time acculated time
    //calculate minutes
    if (accumulatedSec >= 60)
    {
        accumulatedSec%=60;
        accumulatedMin++;
        accumulatedSec += 2;
    }

    //real-time fetched from compiled time constant __TIME__
    //calculate hours
    if (min >= 60)
    {
        min%=60;
        hour++;
    }
    //system run-time acculated time
    //calculate hours
    //if ((accumulatedMin == 60) && (hourlyAdjusted == 0))
    if (accumulatedMin == 60)
    {
        // adjust time by experiments
        //hourlyAdjusted = 1;
        sec -= 20;
        accumulatedMin = 0;
        accumulatedHour++;
    }
    /*
    if (accumulatedMin == 59)
        hourlyAdjusted = 0;
    */
    if (hour >= 24)
    {
        hour=0;
        date++;
        day++;
        if (day >= 7)
            day %= 7;
    }
    switch (month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            monthEndDate = 31;
            break;
        case 2:
            monthEndDate = 28;
            break;
        default:
            monthEndDate = 30;
    }//switch (month)

    if (date > monthEndDate)
    {
        date=1;
        month++;
    }

    if (month > 12)
    {
        month=1;
        year++;
    }
}//proceedClock

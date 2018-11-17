/**********************************************************
 Target MCU & internal clock speed: ATmega328P @ 8 Mhz
 Name    : LEDinterrupt.c
 Author  : Insoo Kim (insoo@hotmail.com)
 Date    : Nov. 4, 2015
 UPDATE  : Oct. 4, 2016
 
 Description: alarm control
 How to upload hex to target MCU using usbtiny programmer:
 
 <For Mac Xcode>
 cd "/Users/insoo/Google Drive/CT(Computational Thinking)/Ard/AVR/Demo328P/firmware"
 
 avrdude -c usbtiny -P usb -p atmega328p -U flash:w:main.hex:i
 
 <For Windows Atmel Studio>
 cd "Atmel Studio\7.0\ATmega328P\BlinkLED\Debug"
 OR cd "C:\Users\insoo\Documents\Atmel Studio\7.0\ATmega328P\BlinkLED\Debug"
 
 avrdude -c usbtiny -P usb -p atmega328p -U flash:w:BlinkLED.hex:i

 HEX size[char]: 238 out of 32K
 
 Ref:
 ** Written for and tested with a custom board with ATmega328P run on 8 Mhz internal osc
 *********************************************************/

#include <avr/interrupt.h>

volatile int timer_overflow_count = 0;

ISR(TIMER0_OVF_vect) {
    if (++timer_overflow_count > 5*4) {   // a timer overflow occurs 4.6 times per second
        // Toggle Port B pin 4 output state
        PORTB ^= 1<<PB0;
        timer_overflow_count = 0;
    }
}

int main(void) {
    // Set up Port B pin 0 mode to output
    DDRB = 1<<DDB0;
    
    // prescale timer to 1/1024th the clock rate
    //TCCR0B |= (1<<CS02) | (1<<CS00);
    // prescale timer to 1/256th the clock rate
    TCCR0B |= (1<<CS02);
    
    // enable timer overflow interrupt
    TIMSK0 |=1<<TOIE0;
    sei();
    
    while(1) {
        // let ISR handle the LED forever
    }
}
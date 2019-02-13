/*
 * Sound.c
 *
 * Created: 2019-01-29 오후 6:07:11
 *  Author: insoo
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include "externs.h"
#include "defines.h"
#include <util/delay.h>

 void buzzerSoundOn()
 {
	BUZZER_port |= _BV(BUZZER_bit);
	_delay_ms(100);
 }//buzzerSoundOn

 void buzzerSoundOff()
 {
	 BUZZER_port &= ~_BV(BUZZER_bit);
	 _delay_ms(1000);
 }//buzzerSoundOff

 void buzzerBeepBeep()
 {
	buzzerSoundOn();
	buzzerSoundOff();
 }//buzzerBeepBeep
/*
 * DHT11_AVR.c
 *
 * Created: 2018-08-13 오후 8:46:46
 * Author: insoo
 * Ref: http://www.electronicwings.com/avr-atmega/dht11-sensor-interfacing-with-atmega16-32
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

 uint8_t c=0;
 //,I_RH,D_RH,I_Temp,D_Temp,CheckSum;

 void Request()				/* Microcontroller send start pulse/request */
 {
	 lcd_DHT11_ddr |= (1<<lcd_DHT11_bit);
	 lcd_DHT11_portOUT &= ~(1<<lcd_DHT11_bit);	/* set to low pin */
	 _delay_ms(20);			/* wait for 20ms */
	 lcd_DHT11_portOUT |= (1<<lcd_DHT11_bit);	/* set to high pin */
 }

 void Response()				/* receive response from DHT11 */
 {
	 lcd_DHT11_ddr &= ~(1<<lcd_DHT11_bit);
	 while(lcd_DHT11_portIN & (1<<lcd_DHT11_bit));
	 while((lcd_DHT11_portIN & (1<<lcd_DHT11_bit))==0);
	 while(lcd_DHT11_portIN & (1<<lcd_DHT11_bit));
 }

 uint8_t Receive_data()			/* receive data */
 {
	 for (int q=0; q<8; q++)
	 {
		 while((lcd_DHT11_portIN & (1<<lcd_DHT11_bit)) == 0);  /* check received bit 0 or 1 */
		 _delay_us(30);
		 if(lcd_DHT11_portIN & (1<<lcd_DHT11_bit))/* if high pulse is greater than 30ms */
		 c = (c<<1)|(0x01);	/* then its logic HIGH */
		 else			/* otherwise its logic LOW */
		 c = (c<<1);
		 while(lcd_DHT11_portIN & (1<<lcd_DHT11_bit));
	 }
	 return c;
 }

 int chkDHT11(uint8_t *humidInt, uint8_t *humidDec,uint8_t *tempInt, uint8_t *tempDec, uint8_t *chkSum)
 {
	//char data[5];
	 
	Request();		/* send start pulse */
	Response();		/* receive response */
	*humidInt=Receive_data();	/* store first eight bit in I_RH */
	*humidDec=Receive_data();	/* store next eight bit in D_RH */
	*tempInt=Receive_data();	/* store next eight bit in I_Temp */
	*tempDec=Receive_data();	/* store next eight bit in D_Temp */
	*chkSum=Receive_data();	/* store next eight bit in CheckSum */
		 
	if ((*humidInt + *humidDec + *tempInt + *tempDec) != *chkSum)
	{
		return (-1);
	}
	else
	{
		//itoa(I_RH,data,10);
		return (0);
	}
		 
	_delay_ms(10);
}
/*
 * _74HC595.c
 *
 * Created: 2017-07-08 오후 3:01:39
 *  Author: insoo
 */ 
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>


#define halfSec 75

#define ST_CP_PORT PORTB
#define ST_CP_PIN  0 //2 //latch
#define SH_CP_PORT PORTB
#define SH_CP_PIN  3 //1 //clock
#define DS_PORT    PORTB
#define DS_PIN     2 //0 //data

#define DS_low()  DS_PORT&=~_BV(DS_PIN)
#define DS_high() DS_PORT|=_BV(DS_PIN)
#define ST_CP_low()  ST_CP_PORT&=~_BV(ST_CP_PIN)
#define ST_CP_high() ST_CP_PORT|=_BV(ST_CP_PIN)
#define SH_CP_low()  SH_CP_PORT&=~_BV(SH_CP_PIN)
#define SH_CP_high() SH_CP_PORT|=_BV(SH_CP_PIN)

//Define functions
//======================
void ioinit(void);
void demo7seg();
void demoDiceOn5by7PerfBd(uint8_t );
void show7seg(unsigned char);
void output_led_state(unsigned char __led_state);
//======================
//** For 7seg
char OnesDigit[12]= {
	0b01111110, //0
	0b00000110, //1
	0b10111100, //2
	0b10101110, //3
	0b11000110, //4
	0b11101010, //5
	0b11011010, //6
	0b00100110, //7
	0b11111110, //8
	0b11100110, //9
	0b00000001, //DP (10)
	0b00000000 //Blank (11)
};
char TensDigit[11] = {
	0b11111010, //0
	0b10010000, //1
	0b01111100, //2
	0b11011100, //3
	0b10010110, //4
	0b11001110, //5
	0b11100110, //6
	0b10011000, //7
	0b11111110, //8
	0b10011110 //9
};//TensDigit[10]

//** For Dice

char diceOnesDigit[12]= {
	0b00000000, //0
	0b00000001, //1
	0b00000010, //2
	0b00000011, //3
	0b11000110, //4
	0b11101010, //5
	0b11011010, //6
	0b00100110, //7
	0b11111110, //8
	0b11100110, //9
	0b00000001, //DP (10)
	0b00000000 //Blank (11)
};
int main (void)
{
	uint8_t k;
	ioinit(); //Setup IO pins and defaults
	
	while(1)
	{
		//demo7seg();
		for (k=0; k<128; k++)
		{
			show7seg(k<<1);
			_delay_ms(500);
		}

	}
}//main


void ioinit (void)
{
	//DDRB  = 0xff; //1 = output, 0 = input
	DDRB  = 0x00; //1 = output, 0 = input
	DDRB |= _BV(ST_CP_PIN); //output
	DDRB |= _BV(SH_CP_PIN); //output
	DDRB |= _BV(DS_PIN); //input
	
	PORTB = 0x00;
}//ioinit

void demoDiceOn5by7PerfBd(uint8_t k)
{
	
	show7seg(_BV(k)); 
	
}//demoDiceOn5by7PerfBd

void demo7seg()
{
	uint8_t i, j;
	
	for (i=0; i<10; i++)
		for (j=0; j<10; j++)
		{
			show7seg(OnesDigit[j]); //Ones
			show7seg(TensDigit[i]); //Tens
			_delay_ms(halfSec);
		}
	
}//demo7seg

void show7seg(unsigned char num)
{
	int i;
	SH_CP_low();
	ST_CP_low();
	for (i=7;i>=0;i--)
	{
		if (bit_is_set(num, i))
		DS_high();
		else
		DS_low();
		
		SH_CP_high();
		SH_CP_low();
	}
	ST_CP_high();
}//show7seg
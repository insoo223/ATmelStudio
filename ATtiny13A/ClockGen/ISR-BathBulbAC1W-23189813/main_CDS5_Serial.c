/**************************************************************
 Target MCU & clock speed: ATtiny13A @ 1.2Mhz internal
 Name    : main.c
 C modules of this project, ISR:
main.c
Custom Headers:
    Nothing
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : Sep 26, 2018
 Updated : Sep 27, 2018 (On Atmel Studio 7)

 Description: 
	ATtiny13A controls power based on PIR motion sensor input. 
	PIR sensor triggers PCINT1

 HEX size[Byte]: 176 out of 1024

How to upload to the target MCU
<For Windows Atmel Studio>
Slect Tool ? USBtiny (USBtiny memu should be configured in the external tool memu)

<For CMD window or DOS prompt>
cd " C:\Users\insoo\Documents\GitHub\ATmelStudio\ATtiny13A\ClockGen\ISR\Debug "
avrdude -c usbtiny -P usb -p attiny13 -U flash:w:ISR.hex:i

 Ref:ADC on ATtiny13a
 https://www.avrfreaks.net/forum/attiny13-i-need-example-working-adc
 https://www.adnbr.co.uk/articles/adc-and-pwm-basics
 ATTiny Port Manipulation (Part 2): AnalogRead()
 https://www.instructables.com/id/ATTiny-Port-Manipulation-Part-2-AnalogRead/
 Attiny13 clock frequency and ADC
 https://www.avrfreaks.net/forum/attiny13-clock-frequency-and-adc
 Problem with ADC on ATTiny13
 https://www.avrfreaks.net/forum/problem-adc-attiny13
 adnbr/attiny13-adc-pwm.c
 https://gist.github.com/adnbr/9289235
 http://www.avrbeginners.net/
*****************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define    UART_RX_ENABLED        (1) // Enable UART RX
#define    UART_TX_ENABLED        (1) // Enable UART TX

#define        F_CPU           (1200000UL) // 1.2 MHz

#if defined(UART_TX_ENABLED) && !defined(UART_TX)
#define        UART_TX         PB3 // Use PB3 as TX pin
#endif  /* !UART_TX */

#if defined(UART_RX_ENABLED) && !defined(UART_RX)
#define        UART_RX         PB4 // Use PB4 as RX pin
#endif  /* !UART_RX */

#if (defined(UART_TX_ENABLED) || defined(UART_RX_ENABLED)) && !defined(UART_BAUDRATE)
#define        UART_BAUDRATE   (19200)
#endif  /* !UART_BAUDRATE */

#define    TXDELAY             (int)(((F_CPU/UART_BAUDRATE)-7 +1.5)/3)
#define RXDELAY             (int)(((F_CPU/UART_BAUDRATE)-5 +1.5)/3)
#define RXDELAY2            (int)((RXDELAY*1.5)-2.5)
#define RXROUNDED           (((F_CPU/UART_BAUDRATE)-5 +2)/3)
#if RXROUNDED > 127
# error Low baud rates unsupported - use higher UART_BAUDRATE
#endif

static char uart_getc();
static void uart_putc(char c);
static void uart_puts(const char *s);

void demoPrint();
void int2ascii(int val);

void adc_setup (void)
{
	// Set the ADC input to PB2/ADC1, left adjust result
	ADMUX |= (1 << MUX0) | (1 << ADLAR);

	// Set the prescaler to clock/128 & enable ADC
	// At 9.6 MHz this is 75 kHz.
	// See ATtiny13 datasheet, Table 14.4.
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
	//ADCSRA &= ~(1 << ADPS2);
	//ADCSRA |= (1 << ADPS1);
	//ADCSRA &= ~(1 << ADPS0);
	//ADCSRA |= (1 << ADEN);

	// Start the first conversion
	ADCSRA |= (1 << ADSC);
}

int adc_read (void)
{
	int adc_result;
	//while (ADSC == 1)
		;
	adc_result = ADCH;
	//adc_result = ADC; // For 10-bit resolution

	// Start the next conversion
	ADCSRA |= (1 << ADSC);

	return adc_result;
}//adc_read 

int
main(void)
{
	//char c, *p, buff[16];
    int adc_in;
	DDRB |= (1<<PB0)|(1<<PB1);
    adc_setup();

	//uart_puts("Hello World! \r\n");
	//uart_puts("¾È³ç ÀÎ¼ö! \r\n");
	//uart_puts("Send to Arduino Serial... \r\n");
	/*
	uart_putc("K");
	uart_putc("I");
	uart_putc("M");
	uart_putc("\n");
	*/
	/* loop */
	while (1) {
		/*
		p = buff;
		while((c = uart_getc()) != '\n' && (p - buff) < 16) {
			*(p++) = c;
			// *(p++) = 'k';
		}
		*p = 0;
		_delay_ms(10);
		uart_puts(buff);
		*/
		adc_in = adc_read();
		if (adc_in < 20)
		{
			PORTB |= (1<<PB0);
			PORTB |= (1<<PB1);
			_delay_ms(10000);
		}
		else
			PORTB &= ~(1<<PB0);
			PORTB &= ~(1<<PB1);
		//int2ascii(adc_in);
		//_delay_ms(200);
		//demoPrint();
	}
}

void
int2ascii(int val)
{
	int i=0, d=0, h=0, k=0;

	if (val>=1000)
		k = val / 1000;
	if (val >=100)
		h = (val - (k*1000)) / 100;
	if (val >=10)
		d = (val - (k*1000) - (h*100)) / 10;
	i = val % 10;

	uart_putc(k + 48);
	uart_putc(h + 48);
	uart_putc(d + 48);
	uart_putc(i + 48);
	//uart_puts("»ç¶ûÇØ, Èñ¼±! \r\n");
	uart_puts("\r\n");
}//int2ascii

void
demoPrint()
{
	static int i=0, d=0, h=0, k=0;
	if (i<10)
	{
		uart_putc(k + 48);
		uart_putc(h + 48);
		uart_putc(d + 48);
		uart_putc(i++ + 48);
		uart_puts(": Hello World! \r\n");
		_delay_ms(200);
	}
	else
	{
		i=0;
		if (d<9)
		d++;
		else
		{
			d=0;
			if (h<9)
			h++;
			else
			{
				h=0;
				if (k<9)
				k++;
				else
				k=0;
			}
		}
	}

}
char
uart_getc(void)
{
	#ifdef    UART_RX_ENABLED
	char c;
	uint8_t sreg;

	sreg = SREG;
	cli();
	PORTB &= ~(1 << UART_RX);
	DDRB &= ~(1 << UART_RX);
	__asm volatile(
	" ldi r18, %[rxdelay2] \n\t" // 1.5 bit delay
	" ldi %0, 0x80 \n\t" // bit shift counter
	"WaitStart: \n\t"
	" sbic %[uart_port]-2, %[uart_pin] \n\t" // wait for start edge
	" rjmp WaitStart \n\t"
	"RxBit: \n\t"
	// 6 cycle loop + delay - total = 5 + 3*r22
	// delay (3 cycle * r18) -1 and clear carry with subi
	" subi r18, 1 \n\t"
	" brne RxBit \n\t"
	" ldi r18, %[rxdelay] \n\t"
	" sbic %[uart_port]-2, %[uart_pin] \n\t" // check UART PIN
	" sec \n\t"
	" ror %0 \n\t"
	" brcc RxBit \n\t"
	"StopBit: \n\t"
	" dec r18 \n\t"
	" brne StopBit \n\t"
	: "=r" (c)
	: [uart_port] "I" (_SFR_IO_ADDR(PORTB)),
	[uart_pin] "I" (UART_RX),
	[rxdelay] "I" (RXDELAY),
	[rxdelay2] "I" (RXDELAY2)
	: "r0","r18","r19"
	);
	SREG = sreg;
	return c;
	#else
	return (-1);
	#endif /* !UART_RX_ENABLED */
}

void
uart_putc(char c)
{
	#ifdef    UART_TX_ENABLED
	uint8_t sreg;

	sreg = SREG;
	cli();
	PORTB |= 1 << UART_TX;
	DDRB |= 1 << UART_TX;
	__asm volatile(
	" cbi %[uart_port], %[uart_pin] \n\t" // start bit
	" in r0, %[uart_port] \n\t"
	" ldi r30, 3 \n\t" // stop bit + idle state
	" ldi r28, %[txdelay] \n\t"
	"TxLoop: \n\t"
	// 8 cycle loop + delay - total = 7 + 3*r22
	" mov r29, r28 \n\t"
	"TxDelay: \n\t"
	// delay (3 cycle * delayCount) - 1
	" dec r29 \n\t"
	" brne TxDelay \n\t"
	" bst %[ch], 0 \n\t"
	" bld r0, %[uart_pin] \n\t"
	" lsr r30 \n\t"
	" ror %[ch] \n\t"
	" out %[uart_port], r0 \n\t"
	" brne TxLoop \n\t"
	:
	: [uart_port] "I" (_SFR_IO_ADDR(PORTB)),
	[uart_pin] "I" (UART_TX),
	[txdelay] "I" (TXDELAY),
	[ch] "r" (c)
	: "r0","r28","r29","r30"
	);
	SREG = sreg;
	#endif /* !UART_TX_ENABLED */
}

void
uart_puts(const char *s)
{
	while (*s) uart_putc(*(s++));
}
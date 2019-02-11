; Target MCU: ATtiny13A 
; Name: blinkPB0_WDT.asm
; Created: Oct 07, 2018
; Updated: Oct 07, 2018
; Author: Insoo Kim (insoo@hotmail.com)
; Desc: Blink LED on PB0 using WDT
; Ref: 

.include "tn13def.inc"

; Register definition
.def rsreg = r18 ; saving the status during intr
.def rmp = r16 ; temp reg outside intr
.def rimp = r17 ; temp reg inside intr

.def regTemp = r19

; Interrupt Trigger Mode constants
.equ mskaExtInt0 = 0b11111100 ; ds 47
.equ mskoExtInt0LvlLow =  0b00000000
.equ mskoExtInt0LvlChg =  0b00000001
.equ mskoExtInt0EdgeH2L = 0b00000010
.equ mskoExtInt0EdgeL2H = 0b00000011


; ISR table
.cseg
.org $0000
	rjmp main ;vector #1 RESET vector, execute at start-up
	rjmp extINT0 ;vector #2 INT0 (unused) vector
	reti ;vector #3 PCINT0 (unused) vector
	reti ;vector #4 TIM0_OVF (unused) vector
	reti ;vector #5 EE_RDY (unused) vector
	reti ;vector #6 ANA_COMP (unused) vector
	reti ;vector #7 TIM0_COMPA (unused) vector
	reti ;vector #8 TIM0_COMPB (unused) vector
	reti ;rjmp wdtISR;vector #9 WDT  vector
	reti ;vector #10 ADC (unused) vector

;----------- ISR ---------------
extINT0:
	;is LED ON, then OFF?
	;Skip (the next instruction) if Bit Is Set
	;sbic PINB, PINB0
	;cbi PORTB, 3 ; turn OFF PB3
	push rimp
	in rimp, SREG 
	push rimp

	sbi PORTB, PB3 ; turn ON PB3

	pop rimp
	out SREG, rimp
	pop rimp

	reti

main:
	cli
	ldi rmp, LOW(RAMEND) ; set stack reg
	out SPL, rmp
	rcall setupPORTB
	rcall setupINT0
	
	sei
	;rcall setupPORTB
	
loop:
	rjmp  loop

setupPORTB:
	sbi	DDRB, DDB3 ; PB3 as output
	;cbi PORTB, PB3
	cbi	DDRB, DDB0
	ret

setupINT0:
	;INT0 as interrupt  (Mask) ds 47
	in 	regTemp, GIMSK 
	;ori	regTemp, (1<<INT0)
	ori	regTemp, 0b01000000
	out GIMSK, regTemp

	;INT0 as interrupt  (Flag) ds 48
	;in 	regTemp, GIFR
	;ori	regTemp, (1<<INT0)
	;ori	regTemp, 0b01000000
	;out GIFR, regTemp

	;Falling edge as wake-up signal
	in 		regTemp, MCUCR
	andi	regTemp, mskaExtInt0
	ori		regTemp, mskoExtInt0EdgeH2L ; rising edge
	;ori		regTemp, mskoExtInt0LvlLow
	out 	MCUCR, regTemp
	ret



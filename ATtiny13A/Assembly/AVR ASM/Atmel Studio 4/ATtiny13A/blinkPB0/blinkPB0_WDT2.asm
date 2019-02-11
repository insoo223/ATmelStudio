; Target MCU: ATtiny13A 
; Name: blinkPB0_WDT.asm
; Created: Oct 07, 2018
; Updated: Oct 10, 2018
; Author: Insoo Kim (insoo@hotmail.com)
; Desc: Blink LED on PB0 using WDT
; Ref: 

.include "tn13def.inc"

; Register definition
.def rsreg = r18 ; saving the status during intr
.def rmp = r16 ; temp reg outside intr
.def rimp = r17 ; temp reg inside intr
 
; ISR table
.cseg
.org $0000
	rjmp main ;vector #1 RESET vector, execute at start-up
	reti ;vector #2 INT0 (unused) vector
	reti ;vector #3 PCINT0 (unused) vector
	reti ;vector #4 TIM0_OVF (unused) vector
	reti ;vector #5 EE_RDY (unused) vector
	reti ;vector #6 ANA_COMP (unused) vector
	reti ;vector #7 TIM0_COMPA (unused) vector
	reti ;vector #8 TIM0_COMPB (unused) vector
	rjmp wdtISR;vector #9 WDT  vector
	reti ;vector #10 ADC (unused) vector

wdtISR: ; watch dog timer interrupt service routine
	in rsreg, sreg ; save status
	LDi	r19, 0x1
	EOR r17, r19 ; toggle LED by exclusive or	
	OUT	PORTB, r17 ; write to Input port affects OFF ot the port
	out sreg, rsreg ; restore status
	reti 

main:
	ldi rmp, LOW(RAMEND) ; set stack reg
	out SPL, rmp ; datasheet p11
	ldi rmp, (1<<WDTIE)
	out WDTCR, rmp ; WDT Intr Enable
	rcall setupPORTB
	rcall setupWDTprescaler
	
loop:
	rjmp  loop

setupPORTB:
	LDi	r16, 0x01
	OUT	DDRB, r16
	OUT	PORTB, r16
	RET

setupWDTprescaler:
	cli ; turn off global interrupt
	wdr ; reset WDT
	in	r16, WDTCR ; copy previous value of WDTCR
	ori	r16, (1<<WDCE) | (1<<WDE) ; WD chang enable, WD sys rst enable
	;ori r16, (1<<WDP1) | (1<<WDP0) ; 0.125s
	;ori r16, (1<<WDP2) ; 0.25s
	ori r16,(1<<WDP2) | (1<<WDP0) ; 0.5s prescale (datasheet p43)
	;ori r16,(1<<WDP2) | (1<<WDP1) ; 1s prescale (datasheet p43)
	;ori r16, (1<<WDP3) ; 4s
	;ori r16, (1<<WDP3) | (1<<WDP0) ; 8s 
	out	WDTCR, r16 ; write to WDTCR 
	sei ; turn on global interrupt
	ret



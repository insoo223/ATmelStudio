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

wdtISR: ; watch dog timer interrupt service routine
	in rsreg, sreg ; save status
	cbi	PORTB, PB3 ; write to Input port affects OFF ot the port
	out sreg, rsreg ; restore status
	reti 


main:
	cli
	ldi rmp, LOW(RAMEND) ; set stack reg
	out SPL, rmp

	ldi rmp, (1<<WDTIE)
	out WDTCR, rmp ; WDT Intr Enable

	rcall setupPORTB
	rcall setupINT0
	rcall setupWDTprescaler
	
	sei
	
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

setupWDTprescaler:
	cli ; turn off global interrupt
	wdr ; reset WDT
	in	r16, WDTCR ; copy previous value of WDTCR
	ori	r16, (1<<WDCE) | (1<<WDE) ; WD chang enable, WD sys rst enable
	;ori r16,(1<<WDP2) | (1<<WDP0) ; 0.5s prescale (datasheet p43)
	;ori r16,(1<<WDP2) | (1<<WDP1) ; 1s prescale (datasheet p43)
	ori r16, (1<<WDP3) ; 4s
	;ori r16, (1<<WDP3) | (1<<WDP0) ; 8s 
	out	WDTCR, r16 ; write to WDTCR 
	sei ; turn on global interrupt
	ret


; Target MCU: ATtiny13A 
; Name: blinkPB0_WDT.asm
; Created: Oct 07, 2018
; Updated: Oct 07, 2018
; Author: Insoo Kim (insoo@hotmail.com)
; Desc: Blink LED on PB0 using WDT
; Ref: 

.include "tn13def.inc"

; Register definition
;.def rsreg = r18 ; saving the status during intr
.def regTemp = r16
.def rmp = r17 ; temp reg outside intr
.def rimp = r18 ; temp reg inside intr

; Power Save Mode constants
; 'mska' is mask to AND with register
; 'msko' is mask to OR with register
.equ mskaPowerMode = 0b11000111 ;AND mask sleep mode
								;bit 5 to be ON to sleep
.equ mskoPowerIdle = 0b00100000
.equ mskoADCreduce = 0b00101000
.equ mskoPowerDown = 0b00110000


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
	
	push rimp
	in rimp, SREG 
	push rimp

	;is LED ON, then OFF?
	;Skip (the next instruction) if Bit Is Set
	sbis PINB, PB3 ; read PB3 state
	rjmp ledON
	cbi PORTB, PB3 ;toggle PB3
	rjmp endINT0
ledON:
	sbi PORTB, PB3 ;toggle PB3
endINT0:
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
	rcall setupSleep
	sei
	
loop:
	sleep
	rjmp  loop

setupSleep:
	in 		regTemp, MCUCR
	andi	regTemp, mskaPowerMode
	;ori		regTemp, mskoPowerDown
	ori		regTemp, mskoPowerIdle
	out 	MCUCR, regTemp
	ret

setupPORTB:
	sbi	DDRB, DDB3 ; PB3 as output
	cbi	DDRB, DDB0 ; PB0 as intput
	ret

setupINT0:
	;INT0 as interrupt  (Mask) ds 47
	in 	regTemp, GIMSK 
	;ori	regTemp, (1<<INT0)
	ori	regTemp, 0b01000000
	out GIMSK, regTemp

	;Falling edge as wake-up signal
	in 		regTemp, MCUCR
	andi	regTemp, mskaExtInt0
	ori		regTemp, mskoExtInt0EdgeH2L ; rising edge
	;ori		regTemp, mskoExtInt0LvlLow ; PPWER DOWN wake up only with level intr
										; ds 30
	out 	MCUCR, regTemp
	ret

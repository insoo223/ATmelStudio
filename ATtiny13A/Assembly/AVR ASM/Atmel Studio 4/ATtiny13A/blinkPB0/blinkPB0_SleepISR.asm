; Target MCU: ATtiny13A 
; Name: blinkPB0_WDT.asm
; Created: Oct 07, 2018
; Updated: Oct 10, 2018
; Author: Insoo Kim (insoo@hotmail.com)
; Desc: Blink LED on PB0 using WDT
; Ref: 

.include "tn13def.inc"

; Register definition
.equ ddrOutput = DDRB
.equ prtOutput = PORTB
.equ pinOutput = PINB

.equ ddrInput = DDRB
.equ prtInput = PORTB
.equ pinInput = PINB

.equ bitOutput = 3 ; output bit for prtOutput
.equ bitInput = 0 ; input bit for prtInput
 
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

; Register nick name
.def regTemp = r16 ; 'high register' is for users
				   ; r16 is the 1st one
 
; ISR table
.cseg
.org $0000
	rjmp main ;vector #1 RESET vector, execute at start-up
	rjmp ExtINT0_ISR;vector #2 INT0 (unused) vector
	reti ;vector #3 PCINT0 (unused) vector
	reti ;vector #4 TIM0_OVF (unused) vector
	reti ;vector #5 EE_RDY (unused) vector
	reti ;vector #6 ANA_COMP (unused) vector
	reti ;vector #7 TIM0_COMPA (unused) vector
	reti ;vector #8 TIM0_COMPB (unused) vector
	reti ;wdtISR;vector #9 WDT  vector
	reti ;vector #10 ADC (unused) vector

main:
	cli	; disable interrupt while setting up

	rcall setupStack
	rcall pwrSaveADC
	rcall pwrSaveAnalogComp
	rcall pwrSaveBOD
	rcall pwrSaveWDT
	rcall pwrSavePort
	rcall setupIO
	rcall setupINT0
	rcall setupSleep
	
	sei ; enable interrupt

loop:
	sleep	; sleep as deep as possible
	rjmp  loop	; go to sleep again

;----------- ISR ---------------
ExtINT0_ISR:
	;is LED ON, then OFF?
	;Skip (the next instruction) if Bit Is Set
	sbis	pinOutput, bitOutput 
	rcall	led_on
	rcall	led_off
	reti

led_off:
	cbi		prtOutput, bitOutput
	ret
led_on:
	sbi		prtOutput, bitOutput
	ret
	
;----------- Subroutines ---------------
setupSleep:
	in 		regTemp, MCUCR
	andi	regTemp, mskaPowerMode
	ori		regTemp, mskoPowerDown
	out 	MCUCR, regTemp
	ret
	
setupINT0:
	;INT0 as interrupt  (Mask) ds 47
	in 	regTemp, GIMSK 
	ori	regTemp, (1<<INT0)
	out GIMSK, regTemp

	;INT0 as interrupt  (Flag) ds 48
	in 	regTemp, GIFR
	ori	regTemp, (1<<INT0)
	out GIFR, regTemp

	;Rising edget as wake-up signal
	in 		regTemp, MCUCR
	andi	regTemp, mskaExtInt0
	ori		regTemp, mskoExtInt0EdgeH2L ; rising edge
	out 	MCUCR, regTemp
	ret

setupIO:
	sbi	ddrOutput, bitOutput
	sbi	prtOutput, bitOutput
	cbi	ddrInput, bitInput
	;***********************************************
	sbi	prtInput, bitInput ; Enable pull-up resistor
	;***********************************************
	ret

pwrSavePort:
	ldi 	regTemp, 0xff ; set all pins to output
	out 	ddrOutput, regTemp
	ldi 	regTemp, 0x00 ; set all pins to low
	out 	prtOutput, regTemp
	ret

pwrSaveWDT: 
	in 		regTemp, MCUSR
	andi 	regTemp, ~(1<<WDRF)
	out		MCUSR, regTemp
	ret

pwrSaveBOD: 
	;--- I tried and failed as of 2018.10.10
	;ldi		regTemp, (1<<BODS) | (1<<BODSE); ds 33
	;ldi		regTemp, (1<<BODS)
	;out 	BODCR, regTemp
	;in 		regTemp, BODCR 
	;andi 	regTemp, ~(1<<BODSE)
	;out 	BODCR, regTemp
	;-------------------------
	in 		regTemp, MCUSR
	andi 	regTemp, ~(1<<BORF)
	out		MCUSR, regTemp
	ret

pwrSaveAnalogComp:
	ldi		regTemp, (1<<ACD) ; ds 80
	out 	ACSR, regTemp
	ret

pwrSaveADC:
	in 		regTemp, ADCSRA ; ds 93
	andi 	regTemp, ~(1<<ADEN)
	out 	ADCSRA, regTemp
	ret

setupStack:
	ldi regTemp, LOW(RAMEND) ; set stack reg
	out SPL, regTemp ; datasheet p11
	ret



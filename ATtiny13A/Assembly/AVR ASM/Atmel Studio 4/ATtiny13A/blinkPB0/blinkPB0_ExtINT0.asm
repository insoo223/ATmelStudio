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

.equ bitOutput =  3; 0b00001000 ;  output bit for prtOutput
.equ bitInput = 0b00000001 ; input bit for prtInput
 
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
;.org INT0addr  
	rjmp extINT0 ;vector #2 INT0 (unused) vector
	reti ;vector #3 PCINT0 (unused) vector
	reti ;vector #4 TIM0_OVF (unused) vector
	reti ;vector #5 EE_RDY (unused) vector
	reti ;vector #6 ANA_COMP (unused) vector
	reti ;vector #7 TIM0_COMPA (unused) vector
	reti ;vector #8 TIM0_COMPB (unused) vector
	rjmp wdtISR;vector #9 WDT  vector
	reti ;vector #10 ADC (unused) vector

;----------- ISR ---------------
extINT0:
	;is LED ON, then OFF?
	;Skip (the next instruction) if Bit Is Set
	;sbic PINB, PINB0
	;cbi PORTB, 3 ; turn OFF PB3
	push regTemp
	in regTemp, SREG 
	push regTemp

	sbi PORTB, PB3 ; turn ON PB3

	pop regTemp
	out SREG, regTemp
	pop regTemp

	reti
	
wdtISR: ; watch dog timer interrupt service routine
	in regTemp, SREG ; save status
	LDi	regTemp, 0x00
	OUT	DDRB, regTemp ; maker PB as Input
	OUT	PORTB, regTemp ; write to Input port affects OFF ot the port
	out SREG, regTemp ; restore status
	reti 
;--------------- MAIN (Start point) -----------------
main:
	cli	; disable interrupt while setting up

	rcall setupIO ; for some reason, after reset PB0 
				  ; pull-up doesn't work without this redundant line

	rcall setupStack
	rcall pwrSaveADC
	rcall pwrSaveAnalogComp
	rcall pwrSaveBOD
	;rcall pwrSaveWDT
	;rcall pwrSavePort
	
	rcall setupIO
	
	;rcall setupINT0
	
	rcall setupWDTprescaler
	
	;rcall setupSleep
	
	sei ; enable interrupt

loop:
	;sleep	; sleep as deep as possible
	;----- Test Success
	;sbi PORTB, PB3 
	;rcall ifPINB0pressedThenOnPB3LEDotherwiseOff
	rjmp  loop	; go to sleep again

;----------- Test in process ---------------
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
	in	regTemp, WDTCR ; copy previous value of WDTCR
	ori	regTemp, (1<<WDCE) | (1<<WDE) ; WD chang enable, WD sys rst enable
	ori regTemp,(1<<WDP2) | (1<<WDP0) ; 0.5s prescale (datasheet p43)
	;ori regTemp,(1<<WDP2) | (1<<WDP1) ; 1s prescale (datasheet p43)
	;ori regTemp, (1<<WDP3) ; 4s
	;ori regTemp, (1<<WDP3) | (1<<WDP0) ; 8s 
	out	WDTCR, regTemp ; write to WDTCR 
	sei ; turn on global interrupt
	ret

;----------- Subroutines ---------------
setupIO:
	ldi 	regTemp, 0b11111110 ; 1: output 0: input
	out		DDRB, regTemp
	rcall 	setPB0_pull_up_input
	ret

ifPINB0pressedThenOnPB3LEDotherwiseOff:
	sbis PINB, PINB0
	rcall OnPB3andJump
	rcall OffPB3
	ret

setPB0_pull_up_input:
	sbi PORTB, PB0 ; set PB0 as high (ds 53)
	cbi DDRB, DDB0 ; and make DDB0 as low
				; then pull-up input activated
	nop
	ret

OnPB3andJump:
	sbi PORTB, PB3 ; make PB3 high
	rjmp  loop	; jump to loop to avoid PB3 low again
	ret

OffPB3:
	cbi PORTB, PB3 ; make PB3 low
	ret

setupSleep:
	in 		regTemp, MCUCR
	andi	regTemp, mskaPowerMode
	ori		regTemp, mskoPowerDown
	out 	MCUCR, regTemp
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



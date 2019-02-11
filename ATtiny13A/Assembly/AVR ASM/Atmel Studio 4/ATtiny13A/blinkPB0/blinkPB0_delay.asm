
.include "tn13def.inc"

Start:
	RCALL On
	LDi	r18, 8
	RCALL Delay
	RCALL Off
	LDi	r18, 255
	RCALL Delay

	rjmp  Start

On:
	LDi	r16, 0x01
	OUT	DDRB, r16
	OUT	PORTB, r16
	RET

OFF:
	LDi	r16, 0x01
	OUT	DDRB, r16
	LDi	r16, 0x00
	OUT	PORTB, r16
	RET

Delay:
;	LDi	r18, 4
	LDi	r19, 2
	LDi	r20, 51
L1:	DEC	r20
	BRNE L1
	DEC	r19
	BRNE L1
	DEC	r18
	BRNE L1
	NOP
	RET



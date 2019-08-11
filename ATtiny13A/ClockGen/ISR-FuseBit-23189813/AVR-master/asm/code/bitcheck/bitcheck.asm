 ;Author: Pronoy Chopra
 ;Bitcheck.asm

 .INCLUDE "../../include/m32def.inc"

 .ORG 0

 ;SETTING THES STACK POINTER
 LDI R16, LOW(RAMEND)
 OUT SPL, R16
 LDI R16, HIGH(RAMEND)
 OUT SPH, R16

 
 LDI R16, 0xFF
 OUT DDRC, R16 ;PORTC IS OUTPUT
 
 LDI R17, 0x00
 OUT DDRD, R17 ;PORTD IS INPUT

 OUT PORTD, R16 ;PULL UPS ACTIVE
 
 AGAIN2:
 OUT PORTC, R16
 CALL DELAY
 
 AGAIN: SBIC PIND, 2
 RJMP AGAIN

 LDI R17, 0xAA
 OUT PORTC, R17
 
 CALL DELAY

 BACK: SBIS PIND, 2
 RJMP BACK
 RJMP AGAIN2


 DELAY:
 LDI R18, 50
 LOOP3: LDI R19, 255
 LOOP2: LDI R20, 255
 LOOP1: DEC R20
 BRNE LOOP1
 DEC R19
 BRNE LOOP2
 DEC R18
 BRNE LOOP3
 RET
; ***********************************************
; Program: Seven Segment Display on Port B
; Description: Cycles through digits 0-9 on a seven-segment display
;              connected to PORTB on ATmega8, with 1 second delay
; ***********************************************

; Define I/O Register Addresses
.equ PORTB, 0x18    ; Port B Data Register
.equ DDRB,  0x17    ; Port B Data Direction Register


; Define Constants for Delay Loops
.equ DELAY_OUTER,  64
.equ DELAY_MIDDLE, 125
.equ DELAY_INNER,  250


.section .text
.global main

.org 0x0000
rjmp main

seven_seg_table:
    .byte 0b00111111, 0b00000110, 0b01011011, 0b01001111
    .byte 0b01100110, 0b01101101, 0b01111101, 0b00000111
    .byte 0b01111111, 0b01101111

main:
    ; Initialize PORTB (0-6) as output
    ldi r16, 0x7F
    out DDRB, r16

    ; Initialize table pointer and index
    ldi r31, hi8(seven_seg_table)
    ldi r30, lo8(seven_seg_table)
    ldi r20, 0   ; index = 0

loop:
    cpi r20, 10
    breq main      

    ; Load segment data for current index
    add r30, r20
    adc r31, 0
    lpm r21, Z         

    out PORTB, r21      ; Output to PORTB (7-segment display)

    rcall delay_1sec    ; Wait 1 second
    inc r20            
    rjmp loop          

; ***********************************************
; Subroutine: delay_1sec
; Description: Creates a delay of approximately 1 second at 8MHz
; ***********************************************
delay_1sec:
    push r17
    push r18
    push r19

    ldi r17, DELAY_OUTER    ; Load outer loop counter

delay_outer:
    ldi r18, DELAY_MIDDLE   ; Load middle loop counter

delay_middle:
    ldi r19, DELAY_INNER    ; Load inner loop counter

delay_inner:
    dec r19                 ; Decrement inner counter
    nop                     ; No operation (placeholder for delay)
    brne delay_inner        ; If not zero, loop back

    dec r18                 ; Decrement middle counter
    brne delay_middle       ; If not zero, loop back

    dec r17                 ; Decrement outer counter
    brne delay_outer        ; If not zero, loop back

    pop r19
    pop r18
    pop r17
    ret

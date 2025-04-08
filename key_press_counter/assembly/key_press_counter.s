; ***********************************************
; Program: Seven Segment Display on Port B
; Description: Cycles through digits 0-9 on a seven-segment display
;              connected to PORTB on ATmega8, with button control on PC0
; ***********************************************

; Define I/O Register Addresses
.equ PINB  , 0x16
.equ DDRB  , 0x17
.equ PORTB , 0x18
.equ PINC  , 0x13
.equ DDRC  , 0x14
.equ PORTC , 0x15

; Define Constants for Delay Loops
.equ DELAY_OUTER  , 3
.equ DELAY_MIDDLE ,125
.equ DELAY_INNER  , 250


.section .text
.global main
.org 0x0000
    rjmp main

; 7-segment display encoding table (0–9)
seven_seg_table:
    .byte 0x3F, 0x06, 0x5B, 0x4F
    .byte 0x66, 0x6D, 0x7D, 0x07
    .byte 0x7F, 0x6F


main:
    ; Initialize PORTB (0-6) as output for 7-segment
    ldi r16, 0x7F
    out DDRB, r16
    
    ; Enable internal pull-up on PC0
    ldi r16, 0x01
    out PORTC, r16
    
    ; is_button_pressed = 0
    ldi r17, 0
    
    ; Initialize Z register with address of lookup table
    ldi r31, hi8(seven_seg_table)
    ldi r30, lo8(seven_seg_table)

    ; Load zero pattern and output to PORTB
    lpm r16,z
    out PORTB,r16

    ; index = 1
    ldi r20, 1

loop:
    ; Read PINC
    in r16, PINC
    andi r16, 0x01       ; Check only bit 0 (PC0)
    breq button_pressed  ; If pressed (0), go to button_pressed
    
    ; Button not pressed, set is_button_pressed = 0 and continue
    ldi r17, 0
    rjmp loop

button_pressed:
    ; Skip if already pressed last time
    cpi r17, 1
    breq loop
    
    ; Set is_button_pressed = 1
    ldi r17, 1
    
    ; Reset index if it reaches 10
    cpi r20, 10
    brne not_overflow
    ldi r20, 0
    
not_overflow:
    ; Save Z pointer
    push r30
    push r31
    
    ; Calculate address of current digit pattern
    add r30, r20
    adc r31, 0
    
    ; Load pattern and output to PORTB
    lpm r21, Z
    out PORTB, r21
    
    ; index++
    inc r20
    
    ; Restore Z pointer
    pop r31
    pop r30
    
    ; Add delay for debouncing
    rcall delay_50ms
    
    rjmp loop

; ***********************************************
; Subroutine: delay_50ms
; Creates ~50ms delay using nested loops
; Preserves: r17, r18, r19
; ***********************************************
delay_50ms:
    push r17
    push r18
    push r19
    
    ldi r17, DELAY_OUTER
outer_loop:
    ldi r18, DELAY_MIDDLE
middle_loop:
    ldi r19, DELAY_INNER
inner_loop:
    dec r19
    nop
    brne inner_loop
    dec r18
    brne middle_loop
    dec r17
    brne outer_loop
    
    pop r19
    pop r18
    pop r17
    ret

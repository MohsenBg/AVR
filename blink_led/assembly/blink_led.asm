; ***********************************************
; Program: Blink LED on Port B
; Description: This program blinks an LED connected to PORTB0 on ATmega8
; ***********************************************

; Define I/O Register Addresses
.equ PORTB, 0x18    
.equ DDRB, 0x17     

; Define Constants
.equ LED_PIN, 1        
.equ DELAY_OUTER, 64
.equ DELAY_MIDDLE, 125
.equ DELAY_INNER, 250

; Program starts here
.org 0x0000
    rjmp main

main:
    ; Set LED pin as output
    ldi r16, LED_PIN
    out DDRB, r16
    
blink_loop:
    ; Turn LED ON
    ldi r16, LED_PIN
    out PORTB, r16
    
    ; Delay for approximately 1 second
    rcall delay_1sec
    
    ; Turn LED OFF
    ldi r16, 0
    out PORTB, r16
    
    ; Delay for approximately 1 second
    rcall delay_1sec
    
    ; Repeat the blinking process
    rjmp blink_loop

; ***********************************************
; Subroutine: delay_1sec
; Description: Creates a delay of approximately 1 second for 8MHz
; ***********************************************
delay_1sec:
    ldi r17, DELAY_OUTER    ; outer loop counter (64)
    
delay_outer:
    ldi r18, DELAY_MIDDLE   ; middle loop counter (125)
    
delay_middle:
    ldi r19, DELAY_INNER    ; inner loop counter (250)
    
delay_inner:
    dec r19
    nop                     ; No operation (placeholder for delay)
    brne delay_inner
    
    dec r18
    brne delay_middle
    
    dec r17
    brne delay_outer
    
    ret
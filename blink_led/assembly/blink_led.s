; ***********************************************
; Program: Blink LED on PORTB0
; Description: This program toggles an LED connected to PORTB0
;              on an ATmega8 with approximately 1-second intervals.
; ***********************************************

; Define I/O Register Addresses
.equ PORTB,        0x18
.equ DDRB,         0x17

; Define Constants
.equ LED_PIN,      0       ; PORTB0 (bit 0)
.equ DELAY_OUTER,  64
.equ DELAY_MIDDLE, 125
.equ DELAY_INNER,  250


.section .text
.global main

; Program starts here
.org 0x0000
    rjmp main

main:
    ; Set PORTB0 as output
    ldi r16, (1 << LED_PIN)    
    out DDRB, r16

blink_loop:
    ; Turn LED ON (set PORTB0 high)
    sbi PORTB, LED_PIN
    
    ; Delay for approximately 1 second
    rcall delay_50ms
    
    ; Turn LED OFF (clear PORTB0 low)
    cbi PORTB, LED_PIN
    
    ; Delay for approximately 1 second
    rcall delay_1sec
    
    ; Repeat the blinking process
    rjmp blink_loop

; ***********************************************
; Subroutine: delay_1sec
; Description: Creates a delay of approximately 1 second at 8MHz
; ***********************************************
delay_1sec:
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
    
    ret                     
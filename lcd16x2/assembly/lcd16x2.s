; Define I/O Register Addresses
;=============================================================================

; Port Definitions
.equ PIND,  0x10
.equ DDRD,  0x11
.equ PORTD, 0x12

.equ PINC,  0x13
.equ DDRC,  0x14
.equ PORTC, 0x15

.equ PINB,  0x16
.equ DDRB,  0x17
.equ PORTB, 0x18

; LCD Connection Configuration
.equ LCD_DATA_DDR,  DDRB
.equ LCD_DATA_PORT, PORTB
.equ LCD_CTRL_DDR,  DDRC
.equ LCD_CTRL_PORT, PORTC
.equ LCD_RS_PIN,    0      ; Register Select pin
.equ LCD_EN_PIN,    1      ; Enable pin

; LCD Command Definitions
.equ LCD_INIT_8BIT,    0x33 ; Initialize LCD (8-bit mode)
.equ LCD_SWITCH_4BIT,  0x32 ; Switch to 4-bit mode
.equ LCD_FUNCTION_SET, 0x28 ; 4-bit, 2-line, 5x8 font
.equ LCD_DISPLAY_ON,   0x0C ; Display ON, cursor OFF
.equ LCD_ENTRY_MODE,   0x06 ; Cursor moves right
.equ LCD_CLEAR_SCREEN, 0x01 ; Clear display
.equ LCD_RETURN_HOME,  0x02 ; Return cursor home

;=============================================================================
; Program Section
;=============================================================================

.section .text
.global main

.org 0x0000
    rjmp main

; Display message (padded with spaces)
message: 
    .byte 0x20, 0x20, 0x48, 0x65 ; "  He"
    .byte 0x6C, 0x6C, 0x6F, 0x20 ; "llo "
    .byte 0x57, 0x6F, 0x72, 0x6C ; "Worl"
    .byte 0x64, 0x21, 0x20, 0x20 ; "d!  "
    .byte 0x00, 0x00, 0x00, 0x00 ; Null terminator

;=============================================================================
; Main Program
;=============================================================================
main:
    rcall lcd_init        ; Initialize LCD

display_loop:
    ; Load address of string into Z register (r30:r31)
    ldi r30, lo8(message)     
    ldi r31, hi8(message)
    rcall lcd_clear
    
scroll_text:
    lpm r24, z+            ; Load byte and post-increment Z
    
    cpi r24, 0            ; Check for end of string
    breq reset_display

    rcall lcd_data        ; Display character

    ldi r24, 100          ; Delay 100ms
    rcall delay_ms

    rjmp scroll_text

reset_display:
    rcall delay_1sec
    rcall lcd_clear
    rjmp display_loop

;=============================================================================
; LCD Functions
;=============================================================================

; lcd_command - Sends command to the LCD
; Input: r24 - command byte
lcd_command:
    push r16             
    push r17
    
    mov r16, r24         ; Copy command to r16
    
    ; Send high nibble first
    mov r17, r16         
    swap r17             ; Swap nibbles
    andi r17, 0x0F       ; Mask to get high nibble
    out LCD_DATA_PORT, r17
    
    cbi LCD_CTRL_PORT, LCD_RS_PIN  ; Command mode (RS = 0)
    
    sbi LCD_CTRL_PORT, LCD_EN_PIN  ; Enable pulse
    ldi r24, 1
    rcall delay_ms
    cbi LCD_CTRL_PORT, LCD_EN_PIN
    
    ; Send low nibble
    andi r16, 0x0F       ; Mask to get low nibble
    out LCD_DATA_PORT, r16
    
    cbi LCD_CTRL_PORT, LCD_RS_PIN  ; Command mode (RS = 0)
    
    sbi LCD_CTRL_PORT, LCD_EN_PIN  ; Enable pulse
    ldi r24, 1
    rcall delay_ms
    cbi LCD_CTRL_PORT, LCD_EN_PIN
    
    ldi r24, 2           ; Command processing delay
    rcall delay_ms

    pop r17              
    pop r16
    ret

; lcd_data - Sends data (character) to the LCD
; Input: r24 - data byte
lcd_data:
    push r16
    push r17
    
    mov r16, r24         ; Copy data to r16
    
    ; Send high nibble first
    mov r17, r16
    swap r17
    andi r17, 0x0F
    out LCD_DATA_PORT, r17
    
    sbi LCD_CTRL_PORT, LCD_RS_PIN  ; Data mode (RS = 1)
    
    sbi LCD_CTRL_PORT, LCD_EN_PIN  ; Enable pulse
    ldi r24, 1
    rcall delay_ms
    cbi LCD_CTRL_PORT, LCD_EN_PIN
    
    ; Send low nibble
    andi r16, 0x0F
    out LCD_DATA_PORT, r16
    
    sbi LCD_CTRL_PORT, LCD_RS_PIN  ; Data mode (RS = 1)
    
    sbi LCD_CTRL_PORT, LCD_EN_PIN  ; Enable pulse
    ldi r24, 1
    rcall delay_ms
    cbi LCD_CTRL_PORT, LCD_EN_PIN
    
    ldi r24, 2           ; Character processing delay
    rcall delay_ms
    
    pop r17
    pop r16
    ret

; lcd_clear - Clears the LCD and returns cursor to home
lcd_clear:
    ldi r24, LCD_CLEAR_SCREEN
    rcall lcd_command

    ldi r24, LCD_RETURN_HOME
    rcall lcd_command
    ret

; lcd_init - Initializes the LCD for 4-bit mode
lcd_init:
    push r16
    push r17
    push r24
    
    ; Wait for LCD to stabilize after power-on
    ldi r24, 100
    rcall delay_ms
    
    ; Configure control pins as output
    in r16, LCD_CTRL_DDR
    ori r16, (1 << LCD_RS_PIN) | (1 << LCD_EN_PIN)
    out LCD_CTRL_DDR, r16
    
    ; Configure data pins as output (lower 4 bits)
    in r16, LCD_DATA_DDR
    ori r16, 0x0F
    out LCD_DATA_DDR, r16
    
    ; 4-bit mode initialization sequence
    ldi r24, LCD_INIT_8BIT
    rcall lcd_command
    
    ldi r24, LCD_SWITCH_4BIT
    rcall lcd_command
    
    ldi r24, LCD_FUNCTION_SET
    rcall lcd_command
    
    ldi r24, LCD_DISPLAY_ON
    rcall lcd_command
    
    ldi r24, LCD_ENTRY_MODE
    rcall lcd_command
    
    ldi r24, LCD_CLEAR_SCREEN
    rcall lcd_command
    
    ldi r24, LCD_RETURN_HOME
    rcall lcd_command
    
    pop r24
    pop r17
    pop r16
    ret

;=============================================================================
; Delay Functions
;=============================================================================

; delay_ms - Delay for specified milliseconds (at 8MHz)
; Input: r24 - number of milliseconds
delay_ms:
    push r16
    push r17
    push r18
    
    mov r16, r24
    
outer_loop:
    ldi r17, 25
middle_loop:
    ldi r18, 40
inner_loop:
    dec r18
    nop
    nop
    nop
    nop
    nop
    brne inner_loop
    
    dec r17
    brne middle_loop
    
    dec r16
    brne outer_loop
    
    pop r18
    pop r17
    pop r16
    ret

; delay_1sec - Approximately 1 second delay (at 8MHz)
delay_1sec:
    push r17
    push r18
    push r19

    ldi r17, 64
    
delay_outer:
    ldi r18, 125
    
delay_middle:
    ldi r19, 250
    
delay_inner:
    dec r19
    nop
    brne delay_inner
    
    dec r18
    brne delay_middle
    
    dec r17
    brne delay_outer

    pop r19
    pop r18
    pop r17
    ret
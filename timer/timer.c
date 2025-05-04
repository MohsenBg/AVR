#define __AVR_ATmega8__ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// 7-segment display digit encoding (Common Cathode)
uint8_t segment_digits[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

volatile uint8_t counter = 0;
volatile uint8_t overflow_count = 0;

// Timer0 initialization: ~1s delay using overflow
void timer0_init() {
    TCCR0 = (1 << CS02) | (1 << CS00); // Prescaler = 1024
    TIMSK = (1 << TOIE0);              // Enable Timer0 overflow interrupt
    TCNT0 = 0;                         // Initial timer value
    sei();                             // Enable global interrupts
}

// Timer0 Overflow Interrupt (~31 overflows ≈ 1 second @ 8MHz)
ISR(TIMER0_OVF_vect) {
    overflow_count++;
    if (overflow_count >= 31) {
        overflow_count = 0;
        counter++;
        if (counter > 9) counter = 0;
    }
}

int main(void) {
    // Set PORTD as output for 7-segment display
    DDRD = 0xFF;
    PORTD = 0x00;

    // Set PB0 as input with pull-up resistor (button)
    DDRB  &= ~(1 << PB0);
    PORTB |=  (1 << PB0);

    timer0_init();

    uint8_t isButtonPressed = 0;

    while (1) {
        // Show current digit
        PORTD = segment_digits[counter];

        // Button handling with simple debounce
        if (!(PINB & (1 << PB0))) { // Button pressed (active low)
            if (!isButtonPressed) {
                counter = 0;            // Reset counter
                isButtonPressed = 1;
                _delay_ms(50);          // Debounce delay
            }
        } else {
            isButtonPressed = 0;        // Button released
            _delay_ms(50);              // Debounce delay
        }
    }
}

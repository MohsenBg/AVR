#define __AVR_ATmega8__
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

int main(void)
{
    // Set all PORTB pins as outputs (for 7-segment display)
    DDRB = 0xFF;
    // Set all PORTC pins as inputs, with pull-ups on PC0
    DDRC = 0x00; 
    PORTC = 0x01;  // Enable pull-up resistor on PC0

    // Seven-segment display patterns for digits 0-9 (common cathode)
    uint8_t seven_seg_table[10] = {
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

    unsigned char index = 1;  // Start with digit 0
    uint8_t is_button_pressed = 0;

    PORTB = seven_seg_table[0];  // Display 0 initially

    while (1)
    {
        // Wait for the button press (PC0 low)
        if (!(PINC & 0x01))  // Button is pressed when PC0 is low
        {
            if (!is_button_pressed)
            {
                // Button press detected, update display
                is_button_pressed = 1;
                PORTB = seven_seg_table[index];  // Display the current digit
                index = (index + 1) % 10;  // Increment index and reset to 0 after 9
                _delay_ms(50);  // Small delay for debouncing
            }
        }
        else
        {
            // Reset the button press flag when button is released
            is_button_pressed = 0;
        }
    }

    return 0;
}
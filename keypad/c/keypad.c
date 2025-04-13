#define F_CPU 8000000UL
#define __AVR_ATmega8__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

// 4x4 Keypad mapping
const uint8_t keypad[4][4] = {
	{'7', '8', '9', '/'},
	{'4', '5', '6', '*'},
	{'1', '2', '3', '-'},
	{' ', '0', '=', '+'}
};

// Seven-segment display lookup table (0–9 and '-')
const uint8_t seven_seg_table[11] = {
	0x3F,  // 0
	0x06,  // 1
	0x5B,  // 2
	0x4F,  // 3
	0x66,  // 4
	0x6D,  // 5
	0x7D,  // 6
	0x07,  // 7
	0x7F,  // 8
	0x6F,  // 9
	0x40   // '-' (default)
};

uint8_t getKey(void);

int main(void)
{
	uint8_t key;

	// Initialize keypad: PB0–PB3 output (rows), PB4–PB7 input (columns)
	DDRB = 0x0F;
	PORTB = 0xF0; // Enable pull-ups on column pins

	// Initialize 7-segment display on PORTD
	DDRD = 0xFF;
	PORTD = 0x00;

	while (1)
	{
		key = getKey();

		if (key >= '0' && key <= '9') {
			PORTD = seven_seg_table[key - '0'];
		}
		else if (key == 0xFF) {
			// No key pressed
		}
		else {
			PORTD = seven_seg_table[10]; // Show '-' for other characters
		}

		_delay_ms(200); // Debounce delay
	}
}

uint8_t getKey(void)
{
	for (uint8_t row = 0; row < 4; row++)
	{
		PORTB |= 0x0F;           // Set all rows high
		PORTB &= ~(1 << row);    // Pull current row low
		_delay_us(5);            // Short delay for signal to settle

		for (uint8_t col = 0; col < 4; col++)
		{
			if (!(PINB & (1 << (col + 4))))  // Check column input
			{
				while (!(PINB & (1 << (col + 4)))); // Wait for release
				return keypad[row][col];
			}
		}
	}

	return 0xFF; // No key pressed
}
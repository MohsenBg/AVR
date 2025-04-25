#define F_CPU 8000000UL
#define __AVR_ATmega8__

#include <avr/io.h>
#include <util/delay.h>


// === LCD Commands ===
#define LCD_INIT_8BIT       0x33  // First step to initialize LCD (8-bit mode)
#define LCD_SWITCH_4BIT     0x32  // Second step: switch to 4-bit mode
#define LCD_FUNCTION_SET    0x28  // 4-bit, 2-line, 5x8 font
#define LCD_DISPLAY_ON      0x0C  // Display ON, cursor OFF
#define LCD_ENTRY_MODE      0x06  // Cursor moves to right
#define LCD_CLEAR_SCREEN    0x01  // Clear display
#define LCD_RETURN_HOME     0x02  // Cursor to home position

// === Pin Definitions (adjust as needed) ===
#define LCD_PORT        PORTB
#define LCD_DDR         DDRB
#define LCD_RS_PIN      PB5
#define LCD_RW_PIN      PB6
#define LCD_EN_PIN      PB7

// === Keypad Pins ===
#define KEYPAD_DDR      DDRD
#define KEYPAD_PORT     PORTD
#define KEYPAD_PIN      PIND

// === Keypad Layout ===
const uint8_t keypads[4][4] = {
  	{'7', '8', '9', '/'},
	{'4', '5', '6', '*'},
	{'1', '2', '3', '-'},
	{'c', '0', '=', '+'}
};

// === Function Prototypes ===
void lcd_init();
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_clear();
uint8_t getKey();

int main() {
    lcd_init();
    lcd_clear();

    while (1) {
        uint8_t key = getKey();
        if (key) {
            if (key == 'c')
                lcd_clear();
            else
                lcd_data(key);
        }
    }

    return 0;
}

// === Get Pressed Key ===
uint8_t getKey() {
    uint8_t row, col;

    KEYPAD_DDR = 0x0F;           // Rows (0-3) as output, Columns (4-7) as input
    KEYPAD_PORT = 0xF0;          // Pull-ups on columns, rows HIGH

    for (row = 0; row < 4; row++) {
        KEYPAD_PORT |= 0x0F;             // Set all rows HIGH
        KEYPAD_PORT &= ~(1 << row);      // Set current row LOW

        _delay_ms(1); // Small delay for signal settling

        for (col = 0; col < 4; col++) {
            if (!(KEYPAD_PIN & (1 << (col + 4)))) {
                while (!(KEYPAD_PIN & (1 << (col + 4)))); // Wait for release
                _delay_ms(10); // Debounce
                return keypads[row][col];
            }
        }
    }

    return 0; // No key pressed
}

// === LCD Functions ===
void lcd_init()
{
    _delay_ms(20); // Wait for LCD to stabilize after power-on
    
    LCD_DDR = 0xFF;

    // Initialization sequence for 4-bit mode
    lcd_command(LCD_INIT_8BIT); // Initialize in 8-bit mode (1st step)
    lcd_command(LCD_SWITCH_4BIT); // Switch to 4-bit mode (2nd step)

    // Function set: 4-bit, 2 lines, 5x8 font
    lcd_command(LCD_FUNCTION_SET);

    // Display ON, cursor OFF
    lcd_command(LCD_DISPLAY_ON);

    // Cursor moves right, no shift
    lcd_command(LCD_ENTRY_MODE);

    // Clear screen and go home
    lcd_command(LCD_CLEAR_SCREEN);
    lcd_command(LCD_RETURN_HOME);
}

void lcd_clear() {
    lcd_command(LCD_CLEAR_SCREEN);
    lcd_command(LCD_RETURN_HOME);
}

void lcd_command(uint8_t cmd) {
    // Send high nibble
    LCD_PORT &= 0xF0;                 // Clear data pins while preserving control pins
    LCD_PORT |= (cmd >> 4) & 0x0F;    // Set high nibble
    LCD_PORT &= ~(1 << LCD_RS_PIN);   // Command mode (RS=0)
    LCD_PORT &= ~(1 << LCD_RW_PIN);   // Write mode (RW=0)
    LCD_PORT |= (1 << LCD_EN_PIN);    // Set EN high
    _delay_us(1);                     // Enable pulse width
    LCD_PORT &= ~(1 << LCD_EN_PIN);   // Set EN low
    _delay_us(50);                    // Command execution time

    // Send low nibble
    LCD_PORT &= 0xF0;                 // Clear data pins
    LCD_PORT |= cmd & 0x0F;           // Set low nibble
    LCD_PORT |= (1 << LCD_EN_PIN);    // Set EN high
    _delay_us(1);                     // Enable pulse width
    LCD_PORT &= ~(1 << LCD_EN_PIN);   // Set EN low
    _delay_ms(2);                     // Command execution time
}

void lcd_data(uint8_t data) {
    // Send high nibble
    LCD_PORT &= 0xF0;                 // Clear data pins
    LCD_PORT |= (data >> 4) & 0x0F;   // Set high nibble
    LCD_PORT |= (1 << LCD_RS_PIN);    // Data mode (RS=1)
    LCD_PORT &= ~(1 << LCD_RW_PIN);   // Write mode (RW=0)
    LCD_PORT |= (1 << LCD_EN_PIN);    // Set EN high
    _delay_us(1);                     // Enable pulse width
    LCD_PORT &= ~(1 << LCD_EN_PIN);   // Set EN low
    _delay_us(50);                    // Data processing time

    // Send low nibble
    LCD_PORT &= 0xF0;                 // Clear data pins
    LCD_PORT |= data & 0x0F;          // Set low nibble
    LCD_PORT |= (1 << LCD_EN_PIN);    // Set EN high
    _delay_us(1);                     // Enable pulse width
    LCD_PORT &= ~(1 << LCD_EN_PIN);   // Set EN low
    _delay_ms(1);                     // Data processing time
}
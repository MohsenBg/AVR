#define __AVR_ATmega8__
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>‍‍‍
#include <avr/interrupt.h>

// Pin definitions
#define LCD_PORT PORTC‍
#define LCD_DDR DDRC
#define LCD_RS 0
#define LCD_EN 1

#define KEYPAD_PORT PORTB
#define KEYPAD_PIN PINB
#define KEYPAD_DDR DDRB

// LCD command definitions
#define LCD_INIT_8BIT           0x33  // First step to initialize LCD (8-bit mode)
#define LCD_SWICH_4BIT          0x32  // Second step: switch to 4-bit mod
#define LCD_FUNCTION_SET        0x28  // 4-bit, 2-line, 5x8 font
#define LCD_DISPLAY_ON          0x0C  // Display ON, cursor OFF
#define LCD_ENTRY_MODE          0x06  // Cursor moves to right
#define LCD_CLEAR_SCREEN        0x01  // Clear display
#define LCD_RETURN_HOME         0x02  // Cursor to home position

// Keypad matrix
const uint8_t keypads[4][4] = {
    {'7', '8', '9', '/'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'c', '0', '=', '+'}};

// Password definition
const char PASSWORD[] = "1234";
#define PASSWORD_LENGTH 4

// Send 4-bit nibble to LCD
void lcd_send_nibble(uint8_t nibble, uint8_t rs)
{
    if (rs)
        LCD_PORT |= (1 << LCD_RS);
    else
        LCD_PORT &= ~(1 << LCD_RS);

    LCD_PORT &= 0xC3;                 // Clear data bits
    LCD_PORT |= (nibble << 2) & 0x3C; // Set data bits

    LCD_PORT |= (1 << LCD_EN);
    _delay_us(1);
    LCD_PORT &= ~(1 << LCD_EN);
    _delay_us(50);
}

// Send command to LCD
void lcd_command(uint8_t cmd)
{
    lcd_send_nibble(cmd >> 4, 0);   // High nibble, RS=0
    lcd_send_nibble(cmd & 0x0F, 0); // Low nibble, RS=0

    if (cmd < 4)
        _delay_ms(2);
}

// Send data to LCD
void lcd_data(uint8_t data)
{
    lcd_send_nibble(data >> 4, 1);   // High nibble, RS=1
    lcd_send_nibble(data & 0x0F, 1); // Low nibble, RS=1
}

// Print a string to LCD
void lcd_print(const char *text)
{
    for (uint8_t i = 0; text[i] != '\0'; i++)
        lcd_data(text[i]);
}

// Clear LCD display
void lcd_clear(void)
{
    lcd_command(LCD_CLEAR_SCREEN);
    _delay_ms(2);
    lcd_command(LCD_RETURN_HOME);
    _delay_ms(2);
}

// Initialize LCD
void lcd_init(void)
{
    LCD_DDR = 0x3F; // Set LCD pins as output
    _delay_ms(15);  // Wait after power-on

    // Initialization sequence for 4-bit mode
    lcd_command(LCD_INIT_8BIT); // Initialize in 8-bit mode (1st step)
    lcd_command(LCD_SWICH_4BIT); // Switch to 4-bit mode (2nd step)

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

// Scan keypad and return pressed key
uint8_t get_key(void)
{
    uint8_t row, col;

    KEYPAD_DDR = 0x0F;  // Rows output, columns input
    KEYPAD_PORT = 0xF0; // Enable pull-ups on columns

    for (row = 0; row < 4; row++)
    {
        KEYPAD_PORT |= 0x0F;             // Set all rows HIGH
        KEYPAD_PORT &= ~(1 << row);      // Set current row LOW
        _delay_us(2);

        for (col = 0; col < 4; col++)
        {
            if (!(KEYPAD_PIN & (1 << (col + 4))))
            {
                while (!(KEYPAD_PIN & (1 << (col + 4))));          
                _delay_ms(20); // Debounce
                return keypads[row][col];
            }
        }
    }
    return 0; // No key pressed
}

// Move cursor to specific position
void lcd_move_cursor(uint8_t row, uint8_t col)
{
    if (row > 1)
        return;
    uint8_t address = row == 0 ? 0x80 + col : 0xC0 + col;
    lcd_command(address);
}

// Check if input password matches stored password
uint8_t check_password(const char *input)
{
    return (strncmp(input, PASSWORD, PASSWORD_LENGTH) == 0);
}

// Variables for password handling
char user_password[PASSWORD_LENGTH + 1];
uint8_t attempts = 0;
uint8_t locked = 0;
uint8_t pos = 0;


// Interrupt Service Routine for INT0 (when key is pressed to reset)
ISR(INT0_vect)
{
    while (!locked)
    {
        lcd_clear();
        lcd_print("Enter Password:");
        lcd_move_cursor(1, 0);
        memset(user_password, 0, sizeof(user_password)); // Clear old password
        pos = 0;

        // Get password input
        while (pos < PASSWORD_LENGTH)
        {
            uint8_t key = get_key();
            if (key)
            {
                if (key == 'c' && pos > 0)
                { // Handle backspace/clear
                    pos--;
                    user_password[pos] = '\0';
                    lcd_move_cursor(1, pos);
                    lcd_data(' ');
                    lcd_move_cursor(1, pos);
                }
                else if (key != 'c')
                { // Handle numeric input
                    user_password[pos] = key;
                    lcd_data('*'); // Show asterisk for security
                    pos++;
                }
            }
        }

        // Check password
        if (check_password(user_password))
        {
            lcd_clear();
            lcd_print("Access Granted");
            _delay_ms(1000);
        }
        else
        {
            attempts++;
            lcd_clear();
            lcd_print("Access Denied");
            lcd_move_cursor(1, 0);

            if (attempts >= 3)
            {
                lcd_print("Too many attempts");
                _delay_ms(2000);
                locked = 1;
            }
            else
            {
                lcd_print("Attempts left: ");
                lcd_data('0' + (3 - attempts));
                _delay_ms(2000);
            }
        }
    }
}




int main(void)
{
    lcd_init();

    MCUCR |= (1 << ISC01); 
    MCUCR |= (1 << ISC00); 
    GICR |= (1 << INT0);   // Enable INT0 interrupt
    sei();                 // Enable global interrupts

    while (1)
    {
        if (locked)
        {
            lcd_clear();
            lcd_print("DEVICE LOCKED");
            while (1); // Lock the device, do nothing here
        }
    }
}


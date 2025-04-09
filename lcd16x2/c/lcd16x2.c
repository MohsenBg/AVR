#define __AVR_ATmega8__
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

// LCD pin configuration
#define LCD_DATA_DDR   DDRB       // Data pins direction register
#define LCD_CTRL_DDR   DDRC       // Control pins direction register
#define LCD_DATA_PORT  PORTB      // Data output port
#define LCD_CTRL_PORT  PORTC      // Control output port

#define LCD_RS_PIN     PC0        // Register Select: 0 = Command, 1 = Data
#define LCD_EN_PIN     PC1        // Enable signal

// LCD command definitions
#define LCD_INIT_8BIT           0x33  // First step to initialize LCD (8-bit mode)
#define LCD_SWICH_4BIT          0x32  // Second step: switch to 4-bit mod
#define LCD_FUNCTION_SET        0x28  // 4-bit, 2-line, 5x8 font
#define LCD_DISPLAY_ON          0x0C  // Display ON, cursor OFF
#define LCD_ENTRY_MODE          0x06  // Cursor moves to right
#define LCD_CLEAR_SCREEN        0x01  // Clear display
#define LCD_RETURN_HOME         0x02  // Cursor to home position

// Function declarations
void lcd_init();
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_clear();

int main(void)
{
    lcd_init();

    uint8_t *str = "  Hello World!  ";
    while (1)
    {
        for (uint8_t i = 0; i < 16; i++)
        {
            lcd_data(str[i]);
            _delay_ms(100);
        }
        _delay_ms(1000);
        lcd_clear();
    }

    return 0;
}

void lcd_init()
{
    _delay_ms(20); // Wait for LCD to stabilize after power-on

    // Set control pins (RS, EN) as output
    LCD_CTRL_DDR |= (1 << LCD_RS_PIN) | (1 << LCD_EN_PIN);

    // Set lower 4 bits of data port as output (4-bit mode)
    LCD_DATA_DDR |= 0x0F;

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

void lcd_clear()
{
    lcd_command(LCD_CLEAR_SCREEN);
    lcd_command(LCD_RETURN_HOME);
}

void lcd_command(uint8_t cmd)
{
    // Send high nibble
    LCD_DATA_PORT = cmd >> 4;
    LCD_CTRL_PORT &= ~(1 << LCD_RS_PIN); // Command mode
    LCD_CTRL_PORT |= (1 << LCD_EN_PIN);  // Enable high
    _delay_us(1);
    LCD_CTRL_PORT &= ~(1 << LCD_EN_PIN); // Enable low
    _delay_ms(2);

    // Send low nibble
    LCD_DATA_PORT = cmd & 0x0F;
    LCD_CTRL_PORT |= (1 << LCD_EN_PIN);
    _delay_us(1);
    LCD_CTRL_PORT &= ~(1 << LCD_EN_PIN);
    _delay_ms(2);
}

void lcd_data(uint8_t data)
{
    // Send high nibble
    LCD_DATA_PORT = data >> 4;
    LCD_CTRL_PORT |= (1 << LCD_RS_PIN);  // Data mode
    LCD_CTRL_PORT |= (1 << LCD_EN_PIN);
    _delay_us(1);
    LCD_CTRL_PORT &= ~(1 << LCD_EN_PIN);
    _delay_ms(2);

    // Send low nibble
    LCD_DATA_PORT = data & 0x0F;
    LCD_CTRL_PORT |= (1 << LCD_EN_PIN);
    _delay_us(1);
    LCD_CTRL_PORT &= ~(1 << LCD_EN_PIN);
    _delay_ms(2);
}

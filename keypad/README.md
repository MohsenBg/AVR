# Keypad-Controlled Seven Segment Display on ATmega8

This program displays digits (0–9) and the minus symbol (`-`) on a seven-segment display based on input from a 4x4 matrix keypad connected to an ATmega8 microcontroller. When a key is pressed on the keypad, the corresponding character is shown on the seven-segment display.

## Hardware Setup

- **Microcontroller**: ATmega8
- **Keypad**: 4x4 matrix keypad connected to **PORTB**
  - **Rows**: PB0–PB3 (configured as outputs)
  - **Columns**: PB4–PB7 (configured as inputs with pull-ups)
- **Seven-Segment Display**: Connected to **PORTD (pins 0–6)** for segment control
- **Resistors**: Current-limiting resistors (e.g., 220Ω) recommended for each segment line

## Program Description

The program uses a lookup table (`seven_seg_table`) that maps numeric digits (0–9) and the `-` character to their corresponding binary patterns for a seven-segment display. The program scans the keypad continuously, and whenever a valid key is pressed, it displays the corresponding digit or symbol on the display.

### Key Functionalities:

1. **Keypad Matrix Scanning**: The rows of the keypad are set low one at a time, and the columns are checked for a low signal to detect which key is pressed. The corresponding character is returned using a predefined keypad map.
2. **Seven-Segment Table**: A lookup table maps characters to segment values (e.g., `0x3F` for `0`, `0x40` for `-`), which are written to **PORTD** to light up the appropriate segments.
3. **Debouncing**: Simple software debouncing is done by waiting for the key release and adding a delay after each keypress.
4. **Fallback Display**: If an unsupported key is pressed, the minus symbol (`-`) is shown as a default.

## Links to Code

- [C Code Version](./c/keypad.c)


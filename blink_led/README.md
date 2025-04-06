# Simple LED Blink Program on ATmega8 (PORTB0)

This simple program blinks an LED connected to the PORTB0 pin of an ATmega8 microcontroller. The LED will turn on for 1 second, then off for 1 second, repeatedly.

## Hardware Setup
- **Microcontroller**: ATmega8
- **LED**: Connected to **PORTB0**
- **Resistor**: A current-limiting resistor is recommended (e.g., 220Ω).

## Program Description
The program toggles the LED on PORTB0, turning it on for 1 second, then off for 1 second, using simple delay loops. The delay is implemented in both Assembly and C versions.

## Links to Code

- [Assembly Code Version](./assembly/blink_led.s)
- [C Code Version](./c/blink_led.c)

## Steps to Compile
1. **Assemble and Compile the Code**:
   - For **Assembly**: Follow the [C Compilation Guide](./assembly/README.md).
   - For **C**: Follow the [C Compilation Guide](./c/README.md).


# Seven Segment Display Counter on ATmega8

This program cycles through the digits 0-9 on a seven-segment display connected to the PORTB pins of an ATmega8 microcontroller. The digits will be displayed one by one, with a 1-second delay between each digit.

## Hardware Setup
- **Microcontroller**: ATmega8
- **Seven-Segment Display**: Connected to **PORTB (pins 0-6)** for digits 0-9
- **Resistor**: A current-limiting resistor is recommended for each segment of the display (e.g., 220Ω).

## Program Description
The program uses a lookup table to display the digits 0-9 on the seven-segment display connected to PORTB. The program continuously cycles through the digits 0-9 with a 1-second delay between each change. The delay is implemented using a nested loop structure, with timing based on the microcontroller's clock.

### Key Functionalities:
1. **Seven-Segment Table**: A lookup table is used to map each digit (0-9) to its corresponding segment pattern for the seven-segment display.
2. **PORTB Initialization**: The pins of PORTB are configured as output, and the segment data is written to PORTB.
3. **Delay**: A delay function is implemented to introduce a 1-second pause between each digit change.

## Links to Code

- [Assembly Code Version](./assembly/seven_segment.s)
- [C Code Version](./c/seven_segment.c)

## Steps to Compile
1. **Assemble and Compile the Code**:
   - For **Assembly**: Follow the [Assembly Compilation Guide](./assembly/README.md).
   - For **C**: Follow the [C Compilation Guide](./c/README.md).


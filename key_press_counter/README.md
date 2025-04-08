# Key Press Counter on ATmega8
This program uses a button connected to the **PORTC (PC0)** pin to cycle through digits 0-9 on a seven-segment display connected to **PORTB** on an ATmega8 microcontroller. Each time the button is pressed, the displayed digit increments, and a 50ms delay is added to prevent bouncing issues.

## Hardware Setup
- **Microcontroller**: ATmega8
- **Seven-Segment Display**: Connected to **PORTB (pins 0-6)** for digits 0-9
- **Button**: Connected to **PORTC (PC0)**, configured with a pull-up resistor
- **Resistor**: A current-limiting resistor is recommended for each segment of the display (e.g., 220Ω).

## Program Description
The program continuously checks if the button is pressed (using `PINC`), and when pressed, it increments the index and updates the seven-segment display accordingly. The digits 0-9 are shown one by one in a cyclic manner. A debouncing mechanism is implemented by setting a flag (`is_button_pressed`) to ensure that the button press is only counted once per press.

### Key Functionalities:
1. **Button Debouncing**: The program ensures that the button is not repeatedly counted as pressed by checking the `is_button_pressed` flag.
2. **Seven-Segment Table**: A lookup table is used to map each digit (0-9) to its corresponding segment pattern for the seven-segment display.
3. **PORTB Initialization**: The pins of PORTB are configured as output, and the segment data is written to PORTB.
4. **Button Handling**: The program reads the state of the button on **PORTC (PC0)** and updates the display when the button is pressed.

## Links to Code

- [Assembly Code Version](./assembly/key_press_counter.s)
- [C Code Version](./c/key_press_counter.c)

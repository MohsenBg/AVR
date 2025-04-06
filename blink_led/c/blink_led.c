#define __AVR_ATmega8__ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


#define LED_PIN PB0

int main(void) {
    // Set the LED pin as an output
    DDRB |= (1 << LED_PIN);

    while (1) {
        // Turn the LED ON
        PORTB |= (1 << LED_PIN);
        _delay_ms(1000);

        // Turn the LED OFF
        PORTB &= ~(1 << LED_PIN);
        _delay_ms(1000);
    }

    return 0;
}
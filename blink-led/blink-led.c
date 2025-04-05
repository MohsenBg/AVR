#define __AVR_ATmega8__
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>


int main(){

    DDRB = 0x01;

    while (1){
        // Enable
        PORTB  = 0x1;
        _delay_ms(1000);

        // Disable
        PORTB = 0x0;
        _delay_ms(1000);
    }

    return 0;
}
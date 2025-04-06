#define __AVR_ATmega8__ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

int main(void)
{
    
    // Set all PORTB pins as outputs
    DDRB = 0xFF;  
    
    // Seven-segment display patterns for digits 0-9 (common cathode)
    uint8_t seven_seg_table[10] = {
        0b00111111,  // 0
        0b00000110,  // 1
        0b01011011,  // 2   
        0b01001111,  // 3
        0b01100110,  // 4
        0b01101101,  // 5
        0b01111101,  // 6
        0b00000111,  // 7
        0b01111111,  // 8
        0b01101111   // 9
    };
    
    while (1) {
        for (unsigned char i = 0; i < 10; i++){
            PORTB = seven_seg_table[i];  
            _delay_ms(1000);             
        }
    }
    
    return 0; 
}
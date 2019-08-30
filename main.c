#define F_CPU 1600000

#include <avr/io.h>
#include <util/delay.h>

int main(){
    
    DDRA = 0xFF;

    while(1){
        PORTA = 0xFF;
        _delay_ms(100);
        PORTA = 0;
        _delay_ms(100);
    }


    return 0;
}
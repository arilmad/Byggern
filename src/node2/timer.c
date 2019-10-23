#include "timer.h"


ISR (TIMER0_OVF_vect) {
    TCNT0 = 256;
}

void timer_init()
{
    TCNT0 = 256;
    TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0A0);
    TCCR0B = (1 << CS02) | (1 << CS00); /*1024 prescalar */
    TIMSK0 = (1 << TOIE0); //allow interrupts on overflow
}
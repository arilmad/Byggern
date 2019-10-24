#include "timer.h"


ISR (TIMER0_OVF_vect) {
    TCNT0 = 256;
}

void timer_init()
{
    TCNT0 = 256;
    TCCR0A = (1 << WGM01) | (1 << COM0A1) | (1 << COM0A0);
    TCCR0B = (1 << CS02) | (1 << CS00); /*1024 prescalar */
    TIMSK0 = (1 << TOIE0); //allow interrupts on overflow
}


// Register TCCR1(A/B)?
// Table 17-2
// Set WGMn3 WGMn2 WGMn1

// Table 17-4
// COMnA1   COMnA0
//   0        0        Normal port operation, OCnA disconnected
//   0        1        Toggle OCnA on compare match
//   1        0        Clear OCnA on compare match, set OCnA at BOTTOM (non-inverting)
//   1        1        Set OCnA on compare match, clear OCnA at BOTTOM (inverting)

// Table 17-6
// Prescaling


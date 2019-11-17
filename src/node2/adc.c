#include "adc.h"

static void (*fp)(uint8_t);

ISR(TIMER0_OVF_vect)
{
    ; // ADC interrupt triggered by overflow.
}

ISR(ADC_vect)
{
    uint8_t adc_reading = ADCH;

    (*fp)(adc_reading); 
    TCNT0 = 0x00;
    TIFR0 |= (1 << TOV0);
}

void adc_init(void (*f)(uint8_t))
{
    fp = f;

    // Initialize timer
    TCNT0 = 0;
    TCCR0A = (1 << COM0B1);
    TCCR0B = (1 << CS02) | (1 << CS00);
    TIMSK0 = (1 << TOIE0);

    // Initialize ADC interrupt on timer0 overflow.
    ADMUX = (1 << REFS0) | (1 << REFS1) | (1 << ADLAR);
    ADCSRB = (1 << ADTS2) | (0 << ADTS0);
    ADCSRA = (1 << ADIE) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADATE);
    ADCSRA |= (1 << ADEN);
}
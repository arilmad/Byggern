#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>

/* (*fp)(uint8_t)
    * Initialize function pointer to be
    * used by the adc interrupt routine.
 */
static void (*fp)(uint8_t);

/* ISR(TIMER0_OVF_vect)
    * The TIMER0 overflow triggers the
    * adc interrupt vector. Nothing 
    * needs to be done here...
 */
ISR(TIMER0_OVF_vect)
{
    ;
}

/* ISR(ADC_vect)
    * Read from ADCH containing the IR-
    * sensor value when TIMER0 overflows. 
    * Feed function from ir.c with this
    * reading. Reset counter and reset
    * TOV0 flag set by TIMER0 overflow.
 */
ISR(ADC_vect)
{
    uint8_t adc_reading = ADCH;

    (*fp)(adc_reading); 
    TCNT0 = 0x00;
    TIFR0 |= (1 << TOV0);
}

/* timer0_init()
    * clk_io / 1024 prescaling
    * Clear OC0B on Compare Match
    * Enable input capture interrupt
    * Initialize counter at 0
 */
void timer0_init()
{
    TCCR0B = (1 << CS02) | (1 << CS00);
    TCCR0A = (1 << COM0B1);
    TIMSK0 = (1 << TOIE0);
    TCNT0 = 0;
}

/* adc_interrupt_init()
    * Internal 2.56V Voltage Reference 
    * with external capacitor at AREF pin.
    * 
    * Results are left shifted, need only
    * 8-bit precision and to read ADCH.
    * 
    * Enable interrupt trigger on TIMER0
    * overflow.
    * 
    * General configurations. Interrupt enable.
    * Division factor 128, auto trigger.
 */
void adc_interrupt_init()
{
    ADMUX = (1 << REFS0) | (1 << REFS1) | (1 << ADLAR);
    ADCSRB = (1 << ADTS2);
    ADCSRA = (1 << ADIE) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADATE);
    ADCSRA |= (1 << ADEN);
}

/* adc_init(void (*f)(uint8_t))
    * Called from ir.c and intialized
    * with the ir_under_threshold fn.
 */
void adc_init(void (*f)(uint8_t))
{
    fp = f;

    timer0_init();
    adc_interrupt_init();
}
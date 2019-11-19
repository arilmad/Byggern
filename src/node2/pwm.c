#include "pwm.h"

#include <avr/io.h>
#include <avr/interrupt.h>


/* pwm_init()
    * Fast PWM mode
    * Clear OCnA/OCnB on Compare Match
    * clk_io / 8 prescaler
    * TOP value 4000 = 20ms * F_CPU / Prescaler
    * Configure PB5 as PWM output.
*/
void pwm_init(uint16_t duty_cycle) 
{
    TCCR1A = (1 << WGM11) | (1 << COM1A1);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); /*8 prescaler */

    ICR1 = 40000;                                       // TOP = 20ms * F_CPU / Prescaler = 40000

    pwm_set_duty_cycle(duty_cycle);

    DDRB |= (1 << PB5);
}

/* pwm_set_duty_cycle()
    * Set output compare register to tune
    * duty cycle.
*/
void pwm_set_duty_cycle(uint16_t duty_cycle)
{
    OCR1A = (int)(duty_cycle / 65536.0 * ICR1);
}
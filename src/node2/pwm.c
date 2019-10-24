#include "pwm.h"

void pwm_init(uint16_t duty_cycle) // Duty cycle relative position
{
    TCCR1A = (1 << WGM11) | (1 << COM1A1);
    TCCR1B = (1 << WGM13) | (1 << WGM12)  | (1 << CS11); /*8 prescaler */
    ICR1 = 40000; // TOP = 20ms * F_CPU / Prescaler = 40000

    pwm_set_duty_cycle(duty_cycle);

    DDRB |= (1 << PB5);
}

void pwm_set_duty_cycle(uint16_t duty_cycle)
{
    OCR1A = (int)(duty_cycle / 65536.0 * ICR1);
}
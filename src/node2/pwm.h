#include <avr/io.h>
#include <avr/interrupt.h>

void pwm_init(uint16_t duty_cycle);
void pwm_set_duty_cycle(uint16_t duty_cycle);
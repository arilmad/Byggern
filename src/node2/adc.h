#include <avr/io.h>
#include <avr/interrupt.h>

void adc_init(void (*f)(uint8_t));
static uint8_t IR_triggered = 0;
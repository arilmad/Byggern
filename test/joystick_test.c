#include <stdint.h>
#include <stdlib.h>
#include "joystick_test.h"
#include "uart.h"

void joystick_test()
{
    volatile char *adc = (char *) 0x1400;
    uint16_t adc_size = 0x400;
    for(int i = 0; i < adc_size; i++)
    {
        int8_t value = adc[i];
        printf("Value of adc[%4d] is %02d\n\r", i, value);
    }
}
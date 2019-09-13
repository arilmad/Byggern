#include "adc.h"
#include "joystick.h"

static uint8_t x_center;
static uint8_t y_center;

void joystick_init( void )
{
    adc_init();
    joystick_set_center();
}

void joystick_set_center( void )
{
    x_center = adc_read(JOY_X);
    y_center = adc_read(JOY_Y);
}

joystick_pos_t read_joystick_pos( void )
{
    joystick_pos_t position;

    position.x = adc_read(JOY_X) - x_center;
    position.y = adc_read(JOY_Y) - y_center;

    return position;
}
#include "joystick.h"

static uint16_t x_center;
static uint16_t y_center;

static int16_t x_max;
static int16_t y_max;
static int16_t x_min;
static int16_t y_min;

static joystick_dir_t x_dir;
static joystick_dir_t y_dir;

const int8_t THRESHOLD = 5;

ISR(INT0_vect)
{
    joystick_button_pressed = 1;
}

void joystick_interrupt_init(void)
{
    GICR |= (1 << INT0);
    MCUCR |= (1 << ISC01);
    MCUCR &= ~(1 << ISC00);
}

void joystick_set_center(void)
{
    x_center = adc_read(JOY_X);
    y_center = adc_read(JOY_Y);
}

void joystick_init(void)
{
    joystick_interrupt_init();
    joystick_set_center();
    x_max = -10;
    y_max = -10;
    x_min = 10;
    y_min = 10;
    x_dir = NEUTRAL;
    y_dir = NEUTRAL;
}

joystick_pos_t joystick_red_pos(void)
{
    joystick_pos_t position;

    position.x = adc_read(JOY_X) - x_center;
    if (abs(position.x) < THRESHOLD)
    {
        position.x = 0;
    }
    position.y = adc_read(JOY_Y) - y_center;
    if (abs(position.y) < THRESHOLD)
    {
        position.y = 0;
    }

    return position;
}

void joystick_set_dir(joystick_pos_t pos)
{
    if (pos.x > 0)
    {
        x_dir = RIGHT;
    }
    else if (pos.x < 0)
    {
        x_dir = LEFT;
    }
    else
    {
        x_dir = NEUTRAL;
    }

    if (pos.y > 0)
    {
        y_dir = UP;
    }
    else if (pos.y < 0)
    {
        y_dir = DOWN;
    }
    else
    {
        y_dir = NEUTRAL;
    }
}

void joystick_calibrate(joystick_pos_t pos)
{
    if (x_dir == RIGHT)
    {
        x_max = max(pos.x, x_max);
    }
    else if (x_dir == LEFT)
    {
        x_min = min(pos.x, x_min);
    }

    if (y_dir == UP)
    {
        y_max = max(pos.y, y_max);
    }
    else if (y_dir == DOWN)
    {
        y_min = min(pos.y, y_min);
    }
}

joystick_pos_t joystick_get_relative_pos(void)
{
    joystick_pos_t position = joystick_red_pos();
    joystick_set_dir(position);
    joystick_calibrate(position);

    if (x_dir == RIGHT)
    {
        position.x = (int)((float)(position.x) / x_max * 100);
    }
    else if (x_dir == LEFT)
    {
        position.x = (int)((float)position.x / abs(x_min) * 100);
    }
    if (y_dir == UP)
    {
        position.y = (int)((float)position.y / y_max * 100);
    }
    else if (y_dir == DOWN)
    {
        position.y = (int)((float)position.y / abs(y_min) * 100);
    }

    return position;
}

joystick_dir_t joystick_get_x_dir()
{
    return x_dir;
}
joystick_dir_t joystick_get_y_dir()
{
    return y_dir;
}

/*
int joystick_button_poll() {
    if (~(PINB) & 1 << PINB2){
        return 1;
    }
    return 0;
}
*/
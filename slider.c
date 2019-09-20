#include "adc.h"
#include "slider.h"

void slider_init(void)
{
    adc_init();
}

slider_pos_t read_slider_pos(void)
{
    slider_pos_t position;

    position.left_pos = adc_read(SLIDER_L);
    position.right_pos = adc_read(SLIDER_R);

    return position;
}

slider_pos_t get_relative_slider_pos(void)
{
    slider_pos_t position;

    position = read_slider_pos();
    slider_pos_t relative_pos;
    relative_pos.left_pos = (int8_t)((float)(position.left_pos) / 255 * 100);
    relative_pos.right_pos = (int8_t)((float)(position.right_pos) / 255 * 100);

    return relative_pos;
}

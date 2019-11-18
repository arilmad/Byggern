#include "slider.h"
#include "adc.h"

slider_pos_t slider_read_pos(void)
{
    slider_pos_t position;

    position.left_pos = adc_read(SLIDER_L);
    position.right_pos = adc_read(SLIDER_R);

    return position;
}
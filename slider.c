#include "adc.h"
#include "slider.h"

void slider_init( void )
{
    adc_init();
}

slider_pos_t read_slider_pos( void )
{
    slider_pos_t position;

    position.left_pos = adc_read(SLIDER_L);
    position.right_pos = adc_read(SLIDER_R);

    return position;
}
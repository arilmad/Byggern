#include "slider.h"
#include "adc.h"

/* slider_pos_t slider_read_pos(void)
    * Reads the position of the sliders.
    * Uses the external analog to digital converter.
    * Returns the position from 0 to 255.
*/
slider_pos_t slider_read_pos(void)
{
    slider_pos_t position;

    position.left_pos = adc_read(SLIDER_L);
    position.right_pos = adc_read(SLIDER_R);

    return position;
}
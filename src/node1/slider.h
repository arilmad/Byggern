/* slider.h
    * This part of the program is used
    * interpret the position of the
    * sliders on the I/O board.
*/

#ifndef __SLIDER_H__
#define __SLIDER_H__

#include <stdint.h>

/* Defines the type of the slider position */
typedef struct
{
    int16_t left_pos;
    int16_t right_pos;
} slider_pos_t;

slider_pos_t slider_read_pos(void);

#endif
#ifndef _SLIDER_H_
#define _SLIDER_H_

typedef struct
{
    int16_t left_pos;
    int16_t right_pos;
} slider_pos_t;

slider_pos_t read_slider_pos(void);
slider_pos_t get_relative_slider_pos(void);

#endif
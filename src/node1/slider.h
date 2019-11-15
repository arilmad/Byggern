#ifndef __SLIDER_H__
#define __SLIDER_H__

typedef struct
{
    int16_t left_pos;
    int16_t right_pos;
} slider_pos_t;

slider_pos_t slider_read_pos(void);
slider_pos_t slider_get_relative_pos(void);

#endif
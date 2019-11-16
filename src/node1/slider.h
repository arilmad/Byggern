#ifndef __SLIDER_H__
#define __SLIDER_H__

typedef struct
{
    int8_t left_pos;
    int8_t right_pos;
} slider_pos_t;

slider_pos_t slider_read_pos(void);
#endif
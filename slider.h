#ifndef _SLIDER_H_
#define _SLIDER_H_

typedef struct{
    uint8_t left_pos;
    uint8_t right_pos;
} slider_pos_t;

void slider_init( void );

slider_pos_t read_slider_pos( void );

#endif
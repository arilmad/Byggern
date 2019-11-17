#include "ir.h"

#define IR_threshold 30

volatile uint8_t game_over_flag = 0;

void ir_under_threshold(uint8_t IR_state)
{
    if (IR_state < IR_threshold)
        game_over_flag = 1;
}

void ir_init()
{
    adc_init(ir_under_threshold);
}

uint8_t ir_get_game_over_flag()
{
    return game_over_flag;
}

void ir_reset_game_over_flag()
{
    game_over_flag = 0;
}
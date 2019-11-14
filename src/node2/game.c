#include "game.h"
#include "../../lib/uart/uart.h"
#define IR_threshold 30

volatile static uint16_t counter = 0; 


void game_score_keeper(uint8_t IR_state)
{
    //printf("Hei\r\n");
    //printf("%d\r\n", IR_state);
    if (!(IR_state < IR_threshold))
    {
        counter = 200;
    }

    else if ((IR_state < IR_threshold) && !counter) 
    {
        game_over_flag = 1;
    }
 
    if(counter){counter--;}
}

void game_init()
{
    adc_init(game_score_keeper);
}
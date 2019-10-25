#include "game.h"

#define IR_threshold 30

static uint8_t score;
volatile static uint16_t counter = 0;
volatile static uint8_t IR_below_threshold = 0;  


void game_score_keeper(uint8_t IR_state)
{
    if (IR_below_threshold && !(IR_state < IR_threshold))
    {
        counter = 200;
        IR_below_threshold = 0;
    }

    else if (!IR_below_threshold && (IR_state < IR_threshold) && !counter) {
        score--;
        printf("%d\r\n", score);
        IR_below_threshold = 1;
    }
 
    if(counter){counter--;}

}

void game_init()
{
    score = 5;
    adc_init(game_score_keeper);
}
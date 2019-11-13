#include <stdint.h>
#include "adc.h"

#ifndef __GAME_H__
#define __GAME_H__

static uint8_t game_over_flag = 0;

void game_score_keeper(uint8_t IR_state);
void game_init();

#endif
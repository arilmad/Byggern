#ifndef __OLED_H__
#define __OLED_H__

#include <stdint.h>

void oled_init(void);
void oled_reset(void);
void oled_pos(uint8_t row, uint8_t col);

void oled_printf(char *string, uint8_t font_size, uint8_t highlight);
void oled_print_bitmap(const unsigned char *bitmap);

void oled_print_welcome_message(void);
void oled_print_centered_message(char *msg, uint8_t char_width, uint8_t row, uint8_t inverted);
void oled_print_final_score(char *score);

#endif
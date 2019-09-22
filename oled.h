#ifndef __OLED_H__
#define __OLED_H__

#define ASCII_OFFSET 32
#include "fonts.h"

#include <stdint.h>
#include <avr/pgmspace.h>

#include "xmem.h"
#include "uart.h"
#include "fonts.h"

void oled_init(void);
void oled_reset(void);
void oled_pos(uint8_t row, uint8_t col);

void oled_printf(char* string, uint8_t font_size, uint8_t highlight);
void oled_print_bitmap(const unsigned char* bitmap);

void oled_set_brightness(uint8_t lvl);

void oled_print_welcome_message(void);


#endif
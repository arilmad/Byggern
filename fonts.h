/*
 * fonts.h
 *
 * Library of fonts
 * Large: 8x8, normal: 5x7 and small: 4x7
 */
#ifndef __FONTS_H__
#define __FONTS_H__

#include <avr/pgmspace.h>

// Font 8x8 - Large

const unsigned char font8[95][8] PROGMEM;
const unsigned char font5[95][5] PROGMEM;
const unsigned char font4[95][4] PROGMEM;


#endif /* __FONTS_H__ */
#include "oled.h"

#define F_CPU 4915200
#include <util/delay.h>



void oled_write_command(uint8_t command)
{
    base_address_t base_address = OLED_COMMAND_ADDRESS;
    xmem_write(command, 0x00, base_address);
}

void oled_write_data(char c)
{
    base_address_t base_address = OLED_DATA_ADDRESS;
    xmem_write(c, 0x00, base_address);
}

void oled_goto_line(uint8_t line)
{
    oled_write_command(0xb0 | (line & 0x07));
}

void oled_goto_column(uint8_t column)
{
    oled_write_command(column & 0x0f);
    oled_write_command(0x10 | column >> 4);
}

void oled_pos(uint8_t row, uint8_t col)
{
    oled_goto_line(row);
    oled_goto_column(col);
}

void oled_clear_line(uint8_t line)
{
    oled_pos(line, 0);
    for (int col = 0; col < 128; col++)
    {
        oled_write_data(0b00000000);
    }
}

void oled_reset(void)
{
    for (uint8_t line = 0; line < 8; line++)
    {
        oled_clear_line(line);
    }
}

void oled_init(void)
{
    oled_write_command(0xae); // display off
    oled_write_command(0xa1); // segment remap
    oled_write_command(0xda); // common pads hardware: alternative
    oled_write_command(0x12); //
    oled_write_command(0xc8); // common output scan direction: com63~com0
    oled_write_command(0xa8); // multiplex ration mode:63
    oled_write_command(0x3f); //
    oled_write_command(0xd5); // display divide ratio/osc. freq. mode
    oled_write_command(0x80); //
    oled_write_command(0x81); // contrast control
    oled_write_command(0x50); //
    oled_write_command(0xd9); // set pre-charge period
    oled_write_command(0x21); //
    oled_write_command(0x20); // Set Memory Addressing Mode
    oled_write_command(0x02); //
    oled_write_command(0xdb); // VCOM deselect level mode
    oled_write_command(0x30); //
    oled_write_command(0xad); // master configuration
    oled_write_command(0x00); //
    oled_write_command(0xa4); // out follow RAM content
    oled_write_command(0xa6); // set normal display
    oled_write_command(0xaf); // display on

    oled_reset();
}

void oled_print(char* c) {
    uint8_t font_val = (int)(c) - ASCII_OFFSET;
    for(int i = 0; i < 8; i++){
        oled_write_data(pgm_read_byte(&(font8[font_val][i])));     
    }
}

void oled_printf(char* string) {
    int i = 0;
    while (1){
        char* c = string[i];
        if(c == '\0'){
            break;
        }
        i++;
        oled_print(c);
    }
}


void oled_print_arrow(uint8_t row, uint8_t col)
{
    oled_pos(row, col);
    oled_write_data(0b01101100);
    oled_write_data(0b01101100);
    oled_write_data(0b00011000);    
    oled_write_data(0b00011000);
    oled_write_data(0b00011000);
}
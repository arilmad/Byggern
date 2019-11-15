#include "oled.h"

#define F_CPU 4915200
#include <util/delay.h>

uint8_t oled_centered_msg_start_column(char *msg, uint8_t char_width)
{
    uint8_t length = 0;
    while (msg[length] != '\0')
    {
        length++;
    }
    return ((128 - length * char_width) / 2);
}

void oled_print_centered_message(char *msg, uint8_t char_width, uint8_t row, uint8_t inverted)
{
    uint8_t col = oled_centered_msg_start_column(msg, char_width);
    oled_pos(row, col);
    oled_printf(msg, char_width, inverted);
}

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

void oled_print(char *c, uint8_t font_size, uint8_t highlight)
{
    uint8_t font_val = (int)(*c) - ASCII_OFFSET;

    for (int i = 0; i < font_size; i++)
    {
        if (highlight)
        {
            oled_write_data(~pgm_read_byte(&(font5[font_val][i])));
        }
        else if (font_size == 8)
        {
            oled_write_data(pgm_read_byte(&(font8[font_val][i])));
        }
        else
        {
            oled_write_data(pgm_read_byte(&(font5[font_val][i])));
        }
    }
}

void oled_printf(char *string, uint8_t font_size, uint8_t highlight)
{
    int i = 0;
    while (1)
    {
        char c = string[i];
        if (c == '\0')
        {
            break;
        }
        i++;
        oled_print(&c, font_size, highlight);
    }
}

void oled_print_bitmap(const unsigned char *bitmap)
{
    uint8_t page = 0;
    oled_pos(page, 0);
    for (uint16_t i = 0; i < (128 * 8); i++)
    {
        oled_write_data(pgm_read_byte(&(bitmap[i])));
        if (!((i + 1) % 128))
        {
            page++;
            oled_pos(page, 0);
        }
    }
}

void oled_print_welcome_message(void)
{
    oled_pos(0, 0);
    oled_printf("Welcome!", 5, 0);
    oled_pos(1, 0);
    oled_printf("Scroll up", 5, 0);
    oled_pos(2, 0);
    oled_printf("or down", 5, 0);
    oled_pos(3, 0);
    oled_printf("to enter", 5, 0);
    oled_pos(4, 0);
    oled_printf("Main Menu", 5, 0);
}

void oled_print_final_score(char *score)
{
    oled_print_centered_message("Final score", 8, 1, 0);
    for (uint8_t blink = 0; blink < 4; blink++)
    {
        oled_clear_line(4);
        _delay_ms(500);
        oled_print_centered_message(score, 8, 4, 0);
        _delay_ms(500);
    }
}

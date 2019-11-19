#include "encoder.h"

#include <avr/io.h>
#include <avr/delay.h>

/* encoder_reset()
    * Toggle !RST to reset encoder
*/
void encoder_reset()
{
    PORTH &= ~(1 << PH6);
    _delay_us(50);
    PORTH |= (1 << PH6);
}

/* encoder_init()
    * Set SEL, !OE and TRST to initialize encoder.
*/
void encoder_init()
{
    DDRH |= (1 << PH3) | (1 << PH5) | (1 << PH6);
    encoder_reset();
}

/* encoder_read()
    * Read procedure as described in motorbox.pdf.
    * Return encoder value.
*/
int16_t encoder_read()
{
    uint8_t high_val, low_val;
    int16_t val;

    PORTH &= ~(1 << PH5); // Set !OE low
    PORTH &= ~(1 << PH3); // Set SEL low

    _delay_ms(0.02);

    high_val = PINK;     // read msb
    PORTH |= (1 << PH3); // Set SEL high

    _delay_ms(0.02);

    low_val = PINK;      // read lsb
    PORTH |= (1 << PH5); // Set !OE high

    val = (int16_t)(((high_val & 0xFF) << 8) | (0xFF & low_val));

    return val;
}
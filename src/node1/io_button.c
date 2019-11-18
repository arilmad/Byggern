#include "io_button.h"

#include <stdlib.h>
#include <avr/io.h>

/* io_button_init()
    * Inizialise the button
    * Sets the given port to be input
 */
void io_button_init()
{
    DDRB &= (~(1 << PB1));
}

/* io_button_get_left_button_press()
    * Polls the state of the button.
*/
uint8_t io_button_get_left_button_press()
{
    return (PINB &(1 << PB1));
}

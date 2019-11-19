/* encoder.h
    * This part of the program communicates
    * with the motor box encoder and is
    * used to read the racket's horizontal 
    * position.
*/

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <stdint.h>

void encoder_reset();
void encoder_init(void);
int16_t encoder_read(void);

#endif
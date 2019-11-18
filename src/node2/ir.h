/* ir.h
    * This part of the program contains the
    * logic associated with the IR-sensor
    * located behind the ping-pong racket.
    * 
    * If the IR-sensor is triggered, a flag
    * is set which might be obtained and reset
    * through the get_ and reset_ fns.
*/

#ifndef __IR_H__
#define __IR_H__

void ir_init();
uint8_t ir_get_ir_triggered_flag(void);
void ir_reset_ir_triggered_flag(void);

#endif
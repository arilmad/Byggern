#define IR_threshold 30

#include "ir.h"
#include "adc.h"

/* ir_triggered_flag
    * Indicates IR-sensor is triggered. Is
    * set by adc interrupt routine. 
*/
volatile uint8_t ir_triggered_flag;

/* ir_under_threshold(uint8_t IR_state)
    * Compares IR value with defined threshold
    * and might set ir_triggered_flag.
*/
void ir_under_threshold(uint8_t IR_state)
{
    if (IR_state < IR_threshold)
        ir_triggered_flag = 1;
}

/* ir_init()
    * Initializes flag and gives adc_init a
    * pointer to ir_under_threshold to run
    * on interrupt routine.
*/
void ir_init()
{
    ir_triggered_flag = 0;
    adc_init(ir_under_threshold);
}

/* ir_get_ir_triggered_flag()
    * Returns the state of the ir_triggered_flag.
    * Of interest to the node2 main loop.
*/
uint8_t ir_get_ir_triggered_flag()
{
    return ir_triggered_flag;
}

/* ir_reset_ir_triggered_flag()
    * Resets the ir_triggered_flag. Of interest 
    * to the node2 main loop.
*/
void ir_reset_ir_triggered_flag()
{
    ir_triggered_flag = 0;
}
#ifndef __BLUETOOTH_H__
#define ___BLUETOOTH_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "q.h"

void bluetooth_init(int ubrr);

int bluetooth_available();

void bluetooth_send(char *str);
const char *bluetooth_read();

#endif
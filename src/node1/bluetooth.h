#ifndef __BLUETOOTH_H__
#define ___BLUETOOTH_H__

void bluetooth_init(int ubrr);

int bluetooth_msg_available();

void bluetooth_send(char *str);
const char *bluetooth_read();

#endif


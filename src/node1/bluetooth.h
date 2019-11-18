#ifndef __BLUETOOTH_H__
#define ___BLUETOOTH_H__

/* Inizialise the bluetooth module */
void bluetooth_init(int ubrr);

/* Check to see if bluetooth is available for read */
int bluetooth_msg_available();

/* Sends a string to the bluetooth module */
void bluetooth_send(char *str);

/* Reads a message from the bluetooth module */
const char *bluetooth_read();

#endif


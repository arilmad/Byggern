/* bluetooh.h
    * This part of the program sets
    * up a  driver which allows for
    * communication between a phone
    * and node 1.
*/

#ifndef __BLUETOOTH_H__
#define ___BLUETOOTH_H__

void bluetooth_init(int ubrr);
int bluetooth_msg_available();
void bluetooth_send(char *str);
const char *bluetooth_read();

#endif


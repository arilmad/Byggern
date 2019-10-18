#include "stdint.h"

#include "../../lib/can/can_driver.h"
#include "../../lib/uart/uart.h"


int main()
{
    can_message_t response;
    int counter = 1000;

    while(1)
    {
        
        if (~(can_message_read(&response))) // Returns 0 when successfully read
        {
            printf("%s%d", "ID ", response.id);
            printf("%s%s\n\r", "says: ", response.data);
        }
        
        while (counter) {counter--;}
        counter = 1000;
    }
    return 0;
}


#include "stdint.h"

int main()
{
    int counter = 1000;

    while(1)
    {
        while (counter) {counter--;}
        counter = 1000;
    }
    return 0;
}


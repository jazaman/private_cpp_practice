#include <iostream>
#include <stdio.h>
#include "popindirectuser.h"

int main(int argc, char* argv[])
{
    long long decimal = 0;

    if(argc > 1)
    {
        if( sscanf(argv[1], "%lld", &decimal ) > 0)
        {
            std::cout << "Number of 1 is: " << passtopop(decimal) << std::endl;
        } 
    }
    return 0;
}

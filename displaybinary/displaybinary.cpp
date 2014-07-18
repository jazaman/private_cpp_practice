#include <iostream>
#include <cstdio>
#include <stdint.h>

template <typename T>
void display(T aByte)
{
    int loopend = sizeof(aByte) * 8;
    int counter = 0;
    int mask = 1;
     
    for(int i = 0; i < loopend; ++i )
    {
        if( !(i % 8) && i != 0)
        {
            printf(" ");
        }

        printf("%d", (aByte & (mask << i) ) ? 1 : 0 );
    }
    printf("\n");
}


template <typename T>
void display(T bytes, T mask)
{
    
    if( sizeof(bytes) * 8 != mask )
    {
        display( bytes, mask << 1);
    }

    
    printf(" %d [%d] ", (bytes & mask) ? 1 : 0, mask );
}

int main()
{
    uint32_t a = 10;
    uint32_t mask  = 1;

    a = a << 2;
    printf("%d = ", a); 
    display( a);

    a = a >> 2;
    printf("%d = ", a); 
    display( a);
    
    uint32_t a32 = 255;
    printf("%d = ", a32); 
    display( a32);
   

    a = 300;
    
    printf("%d = ",a);
    display(a, mask);
    printf("\n");
    
    display(0xffe);
    printf("\n");
    

    return 0;
}

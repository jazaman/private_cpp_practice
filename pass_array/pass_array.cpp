#include <stdio.h>
#include <iostream>

struct holder {

    char aChar;
    int  aInt;
};

void update_holder(holder** pHolder, size_t size)
{
    for(unsigned i = 0; i < size; i++) {
        
        (*pHolder)[i].aChar = 65+i;
        (*pHolder)[i].aInt = i+99;
    } 
}

int main(int argc, char *argv[])
{
    holder holders[5];

    holder *pH = &holders[0];


    update_holder(&pH, sizeof(holders)/sizeof(holder));
    for(unsigned i = 0; i < sizeof(holders)/sizeof(holder); i++) {
        printf("%d char=%c, int=%d\n", 
            i, holders[i].aChar, holders[i].aInt);
    }
    
    return 0;
}

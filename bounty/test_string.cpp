#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

/*function declaration*/
void reverse(char *src, char *dest);

int main(void) {

    //char array[] = {'a','b','c', '\0'};
    int  intarray[] = {1,2};
    void *array = malloc(32) ;
    memcpy(array, intarray, 16);
    void *ptr = array;
    void **p_ptr  = reinterpret_cast<void**>(ptr+1);
    void **p_ptr1 = reinterpret_cast<void**>(ptr)+1;

    printf("sizeof array:%lu, sizeof char:%lu, sizeof ptr:%lu", 
        sizeof(array)/sizeof(char),
        sizeof(char),
        sizeof(ptr));
    printf("\nstart:%s\n", array );

    printf( "array@%p, ptr->%p, ptr@%p\n", array, ptr, &ptr);
    printf( "reinterpreted ptr:%p->%p\n", p_ptr, *p_ptr);
   
    *p_ptr = 0;
   
    //for(int i = 0; i < sizeof(array)/sizeof(array[0]); i++)
    {
      //  std::cout << "-" <<  array[i]; 
      //  printf("%c.", array[i]);
    } 
    //printf("\nbye:%s\n", array );
    return 0;
}


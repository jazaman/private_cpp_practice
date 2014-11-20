#include <iostream>
#include <stdio.h>

void* global_ptr = 0;

void destroy(void** p = 0)
{
    printf("1.p = %p\n", *p);
    std::swap(*p, global_ptr);
}

void destroy(void* p = 0)
{
    printf("2.p = %p\n", p);
    std::swap(p, global_ptr);
}

int main()
{
    int *p_int = new int[10];
    printf("Initial address for p_int:%p\n", (void*)p_int);
    printf("Initial address for global_ptr:%p\n", global_ptr);
    destroy((void**)&p_int);
    printf("Intermediate address for global_ptr:%p\n", global_ptr);
    printf("Intermediate address for p_int:%p\n", (void*)p_int);
    destroy((void*)p_int);
    printf("Final address for global_ptr:%p\n", global_ptr);
    printf("Final address for p_int:%p\n", (void*)p_int);

    std::cout << "Hello World!!" << std::endl;
    return 0;
}

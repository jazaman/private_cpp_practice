#include <iostream>
#include <stdio.h>
#include <memory>

void* global_ptr = 0;

void destroy(void** p = 0)
{
    printf("1.swapping p = %p with %p\n", *p, global_ptr);
    std::swap(*p, global_ptr);
}

void destroy(void* p = 0)
{
    printf("2.swapping p = %p with %p\n", p, global_ptr);
    std::swap(p, global_ptr);
}

template <typename T>
struct wrapper {
    std::shared_ptr<wrapper<T>> ptr;

    wrapper(T&& ref)
        : refCount(0), obj(ref)
    {
        printf("%d",obj);
        printf("- ctor: %p\n", (void*)&obj);
//        printf("ctor: ");
    }

    
    ~wrapper() {
        printf("%d",obj);
        printf("- dtor: %p\n", (void*)&obj);
//        printf("dtor: ");
    }

    int refCount;
    T& obj;

};

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

    std::shared_ptr<wrapper<int>> ptr_wrapper1 = std::make_shared<wrapper<int>>(99); 
    std::shared_ptr<wrapper<int>> ptr_wrapper2 = ptr_wrapper1;
    ptr_wrapper1->ptr = ptr_wrapper2;
    return 0;
}

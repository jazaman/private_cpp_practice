#include <iostream>
#include <stdio.h>
#include <memory>

void* global_ptr = 0;

void local_swap(void** p = 0)
{
    printf("1.swapping p = %p with %p\n", *p, global_ptr);
    std::swap(*p, global_ptr);
}

void local_swap(void* p = 0)
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

struct Holder {

    int     number;
    const char*   ptr; 

    Holder() : number (99), ptr("ptr test") {}
    void manipulate(Holder& data);

};

void Holder::manipulate(Holder& data) 
{
    Holder* holder_ptr = &data;

    for( auto *i = &(this->number), *end = &(this[1].number); i != end; i++) {
        (*i) = *i + 1;
    }
}

int main()
{
    int *p_int = new int[10];
    printf("Initial address for p_int:%p\n", (void*)p_int);
    printf("Initial address for global_ptr:%p\n", global_ptr);
    local_swap((void**)&p_int);
    printf("Intermediate address for global_ptr:%p\n", global_ptr);
    printf("Intermediate address for p_int:%p\n", (void*)p_int);
    local_swap((void*)p_int);
    printf("Final address for global_ptr:%p\n", global_ptr);
    printf("Final address for p_int:%p\n", (void*)p_int);

    std::cout << "Now testing shred ptr architect !!" << std::endl;

    std::shared_ptr<wrapper<int>> ptr_wrapper1 = std::make_shared<wrapper<int>>(99); 
    std::shared_ptr<wrapper<int>> ptr_wrapper2 = ptr_wrapper1;
    ptr_wrapper1->ptr = ptr_wrapper2;

    Holder data1, data2;
    data1.manipulate(data2);
    printf("Data1 number = %d, data2 number = %d\n", data1.number, data2.number);

    return 0;
}

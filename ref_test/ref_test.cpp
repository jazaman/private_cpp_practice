#include <iostream>

int main()
{
    int *a_ptr = 0;
    int _int = 1;
    int &a_ref = *a_ptr;

    std::cout << "Address pointed by the ptr: " << a_ptr << std:: endl;
    std::cout << "Address pointed by the ref: " << &a_ref << std:: endl;

    a_ptr  = &_int;
    std::cout << "Address pointed by the ptr: " << a_ptr << std:: endl;
    //&a_ref = a_ptr; 
    std::cout << "Address pointed by the ref: " << &a_ref << std:: endl;

    std::cout << "Ptr = " << a_ptr <<" Ref = "<< a_ref << std::endl;
    return 0;
}

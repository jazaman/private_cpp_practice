#include "any.h"
#include <iostream>



int main()
{
    int a= 5;

    Any any(&a);
    
    std::cout << "Hello World!!" << std:: endl;
    std::cout << any.anything << std::endl;
    
    return 0;
}

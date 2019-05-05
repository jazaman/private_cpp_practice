#include "any.h"
#include <iostream>
#include <vector>



int main()
{
    int a= 5;
    double b = 5.5;
    char c = 'c';

    Any any(a);
    std::vector<Any> anything;

    anything.push_back(any);
    anything.push_back(Any(b));
    anything.push_back(Any(c));
    
    std::cout << "Hello World!!" << std::endl;
    for(Any any: anything) {
       std::cout << any_cast<decltype(any)>(any) << std::endl;
    } 
    return 0;
}

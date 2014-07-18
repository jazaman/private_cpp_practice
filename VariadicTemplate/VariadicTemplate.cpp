#include <iostream>
#include <chrono>
#include <thread>

template<typename... T>
void functionTemplateTest(T...)
{
    auto counter = sizeof...(T);
    for( unsigned int i = 0; i < counter; i++)
    {
        std::cout << "-" ;
    }

    std::cout << "Counter:"<< counter << std::endl;
}


int main()
{
    std::cout << "Learning the Variadic Template" << std::endl;

    functionTemplateTest(1, 2, 3);
    functionTemplateTest("hf", 0);


    return 0;
}

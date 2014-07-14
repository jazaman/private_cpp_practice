#include <iostream>
#include <stdint.h>
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>

std::vector<int> gVector;

int print_function(const char* c_str)
{
    char* end = nullptr;
    char* start = const_cast<char*>(c_str);
    
    if( (end = strchr( start, '\0')) != nullptr )
    {
        for(;start != end; start++)
        {
            printf("%c", *start);
        }
        printf("\n");
    }

    return 0;
}

void vector_test(int anInt)
{
    auto i = 2;

    for( i = gVector.begin(); i != gVector.end(); ++i )
    {
    
        std::cout << *i ;

    }
    std::cout << std::endl;
}


int main()
{


    std::string aString("A simple string");
    std::cout << aString << std::endl;

    print_function(aString.c_str()); 

    return 0;
}

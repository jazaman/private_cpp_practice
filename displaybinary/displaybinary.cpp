#include <iostream>
#include <cstdio>
#include <stdint.h>
#include <boost/program_options.hpp>
#include <string>

namespace po = boost::program_options;

template<typename T>
struct deduceType
{
    typedef T deduced_type;
};

template <typename T>
void display(T aByte)
{
    int loopend = sizeof(aByte) * 8;
    int counter = 0;
    int mask = 1;
     
    for(int i = 0; i < loopend; ++i )
    {
        if( !(i % 8) && i != 0)
        {
            printf(" ");
        }

        printf("%d", (aByte & (mask << i) ) ? 1 : 0 );
    }
    printf("\n");
}


template <typename T>
void display(T bytes, T mask)
{
    
    if( ((T)1 << (sizeof(bytes) * 8 -1 )) != mask ) // 
    {
        display( bytes, mask << 1);
    }

#ifdef DEBUG    
    printf(" %d [%d] ", (bytes & mask) ? 1 : 0, mask );
#else
    printf("%d", (bytes & mask) ? 1 : 0 );
#endif
    if( mask == (T)1 << 8 || mask == (T)1 << 16 || mask == (T)1 << 24 ||
        mask == (T)1 << 8*4 || mask == (T)1 << 8*5 || mask == (T)1 << 8*6 ||
        mask == (T)1 << 8*7 )
    {
        printf(" ");
    }
}

int main(int argc, char* argv[])
{
    unsigned long long holder = 0;
    std::string hexHolder;
    std::string format = "";
    //program options
    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "help message")
        ("hex,x",  po::value<std::string>(&hexHolder), "take input in hex")
    ;

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm);

    if(argc > 1 || vm.count("hex") )
    {
        int result = -1;
        int index = 1;
        
        if(vm.count("hex"))
        {
            index = 2;
            format = "%llx";
        }
        else
        {
            format = "%d";
        }
   
        if(( result = sscanf( argv[index], format.c_str(), &holder)) > 0)
        {
             display(holder, (decltype(holder))1);
             printf("\n");
             return 0;
        } 
    }
    int32_t a = 10;
    int32_t mask  = 1;

    a = a << 2;
    printf("%d = ", a); 
    display( a);

    printf("\n"); 
    return 0;
}

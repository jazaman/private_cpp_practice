#include <iostream>


int main()
{
    int data[] = {120, 129, 127, 132};
    int sum = 0;

    for ( int ints : data )
    {
        std::cout << "data: "<< std::dec << ints << std::endl;
    
        if( ints >= 128 ) {
            sum += ints;
            std::cout << "Sum: 0x"<< std::hex << sum << std::endl;
        }        
    }
    
    sum = 0;
    std::cout <<" --------------------------------------------- " << std::endl;

    for( int ints : data)
    {
        std::cout << "data: "<< std::dec << ints << std::endl;
    
        int t = (ints - 128);
        std::cout <<  "t: " << std::hex<< t << " -- " << ~t << std::endl;
        t >>= 31;
        std::cout <<  "t: " << std::hex<< t << " -- " << ~t << std::endl;
        std::cout <<  "t: " << std::hex<< t << " -- " << ~t << std::endl;
        sum += ~t & ints;
        std::cout << "Sum: 0x"<< std::hex << sum << std::endl;
    }



    return 0;
}

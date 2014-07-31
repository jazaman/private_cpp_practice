#include <iostream>
#include <vector>

int main()
{
    std::vector<int> ints = {3,4,6,7,0};
    for( int i : ints)
    {
        std::cout << "'" << i << "' ";
    }
    std::cout << std::endl;
    return 0;
}


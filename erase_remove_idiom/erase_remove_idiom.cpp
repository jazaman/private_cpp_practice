#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

template<class T>
struct typed  {
    typedef T                    value_type;
    typedef T*                   ptr_type;
    typedef std::shared_ptr<T>   smrt_ptr_type;
};


template<typename T>
void print(std::vector<T>& container) {
    for( T &i : container) {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
} 

int main(int argc, char *argv[]) {

    std::string input;

    std::cout << "Enter numbers split by space ...\n";

    std::getline(std::cin, input);

    std::istringstream iss(input);

    int tmp;
    std::vector<std::shared_ptr<int>> holder; //unnecessary type programmng clutter

    while ( iss >> tmp ) {
        //holder.push_back(std::make_shared<typename typed<int>::value_type>(tmp));
        holder.push_back(std::make_shared<int>(tmp));
//        holder.push_back(typename typed<int>::smrt_ptr_type(tmp));
    }

    print(holder);

    std::cout << "Vector size: " << holder.size() << std::endl;

    for( auto& iptr: holder ) {
        if(*iptr == 5) {
             iptr.reset();
        }
    }

    holder.erase(std::remove(std::begin(holder), std::end(holder), std::shared_ptr<int>()), std::end(holder));
    print(holder);
    std::cout << "Vector size: " << holder.size() << std::endl;

    std::cout << std::endl;

    

    return 0;
}

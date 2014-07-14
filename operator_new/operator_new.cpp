#include<iostream>
#include<new>
#include<string>


class A {

public:
    A(const std::string& msg) { 
        std::cout << "A::A("<< msg <<")" << std::endl;
    }
    
    A() { 
        std::cout << "A::A()" << std::endl;
    }
    void* operator new(size_t q_size) {
        
        std::cout << "Mem Alloc Req" << std::endl;

        return  malloc(q_size);
    }

    void print( ) {        
//        std::cout << "A::print()" << std::endl;
    }

    ~A() {
    
        std::cout << "A::~A()" << std::endl;
    }
};


// replacement of a minimal set of functions:
void* operator new(std::size_t sz) throw(std::bad_alloc) {
    std::printf("global op new called, size = %lu\n",sz);
    return std::malloc(sz);
}

int main() {

    std::cout << "Sizeof(A) = " << sizeof(A)  << std::endl;

    A a("a");
//    a.print();

    A* pa = new A("pa");
  //  pa->print();
 
    A* pArray = new A[5];

    A* pb = pArray;

 //   pb->print();

    std::cout << "Hello World" << std::endl;

}

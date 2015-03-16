#include <iostream>

//lvalue reference
void overloaded(int &arg) {
    std::cout << "by lvalue\n";
}

//const lvalue reference
void overloaded(int const &arg) {
    std::cout << "by const lvalue\n";
}

//rvalue reference
void overloaded(int && arg) {
    std::cout << "by rvalue\n";
}

//universal reference
template<typename T>
void overloaded(T && arg) {
    std::cout << "by universal reference\n";
}


template<typename T>
void forwarding(T && arg ) {
    std::cout << "via std::forward: ";
    overloaded( std::forward< T >( arg ) );
    std::cout << "via std::move: ";
    overloaded( std::move( arg ) );
    std::cout << "by simple passing: ";
    overloaded( arg );
}

int main() {
    std::cout << "initial caller passes rvalue:\n";
    forwarding(5);
    std::cout << "initial caller passes lvalue:\n";
    int x = 5;
    forwarding(x);
}

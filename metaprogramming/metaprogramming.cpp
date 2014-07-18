#include <iostream>

template <int N> 
struct Factorial {
    static const int result = N * Factorial<N-1>::result;
};
 
template <> 
struct Factorial<0> {
    static const int result = 1;
};


template <int Num, int Den>
struct Fraction {
     static const int num = Num;
     static const int den = Den;
};

template<int N, typename F>
struct ScalarMultiplication {
    typedef Fraction<N*F::num, N*F::den> result;
};


template<int a, int b>
struct GCD {

    static const int result = GCD<b%a, a>::result; 
};

template<int a>
struct GCD<0,a> {

    static const int result = a;
};

template <typename F>
struct simplified {

    static const int gcd =  GCD<F::num, F::den>::result;
    typedef Fraction<F::num / gcd, F::den/ gcd> Simplified;
};

int main() {
    std::cout << Factorial<5>::result << "\n";

    typedef Fraction<2,3> two_thirds;
    typedef ScalarMultiplication<2, two_thirds>::result four_sixths;


    constexpr int fraction[] = {9,1024};

    std::cout << "The result is " << four_sixths::num << "/" << four_sixths::den<< std::endl;
    std::cout << "GCD for " << fraction[0] << " and " 
                            << fraction[1] <<" is:" 
                            << GCD<fraction[0],fraction[1]>::result 
                            << std::endl;

    typedef Fraction<66, 121> toBeSimplified;
    
    std::cout << "Simplyfying " 
              << simplified<toBeSimplified>::Simplified::num
              << "/"
              << simplified<toBeSimplified>::Simplified::den
              << std::endl;

    
    return 0;
}

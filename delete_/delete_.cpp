#include <iostream>
#include <stdio.h>
#include <iomanip>


class A
{

public:

    A()
    {
         std::cout << "A()" << std::endl;
    }

    ~A()
     {
         std::cout << "~A()" << std::endl;
     //    delete this; 
     }

     void a_function()
     {
         std::cout << "a_function()" << std::endl;
         delete this;
     }

};


void infinity()
{
    
    int x = 0;
    int y = 0;

    while( x < 10 )
    { 
        y = x++;

//        std::cout << std::setprecision(2);
        std::cout << std::setw(9);

        std::cout << "x =" << x 
                  << " y =" << y 
                  << std::endl
                  ;
        std::cout << std::setw(10);
        std::cout << "no " <<  77 << std::endl;
       
        //printf(" -- X = %2d y = %2d\n", x , y);
    }
}


void infinity2()
{
    
    int x = 0;

    while( x < 10 )
    { 
        x = x++; 
    }
}


class B
{

public:
    B()
    {
        std::cout << "B::B()" << std::endl;
    }
    
    ~B()
    {
        std::cout << "B::~B()" << std::endl;
    }
};


int main()
{
    

    A a;

    B b;

    B *bp = &b;

    delete bp;

    bp->~B();


    b.~B();
    //    a.a_function();
    //infinity(); 
    //infinity2(); 

    return 0;

}

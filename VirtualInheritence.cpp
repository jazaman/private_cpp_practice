#include <iostream>
class base
{
public :

    base()
    {
        std::cout<<"base()" << std::endl;
    }
    virtual void fun()
    {    
        std::cout<<"base" << std::endl;
    }
};

class der1: virtual public base
{
public :
    void fun()
    {
        std::cout<<"der1" << std::endl;
    }
};

class der2 : virtual public base
{
public :
    void fun()
    {
        std::cout<<"der2" << std::endl;
    }
};

class der3 : public der1,public der2
{
    public :
    void fun()
    {
        std::cout<<"der3" << std::endl;
    }    
};
int main()
{
    base *p=new der3;
    p->base::fun();
    return 0; 
}

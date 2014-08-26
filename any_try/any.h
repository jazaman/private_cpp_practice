#ifndef _ANY_
#define _ANY_

#ifdef DEBUG
#include <iostream>
#endif

class Placeholder 
{ };

template<typename T>
class Holder: public Placeholder
{
public:
    Holder(T& _anything)
      :  anything(_anything)
    {
#ifdef DEBUG
      std::cout <<"Held:"<< anything << " at:"<< &anything << std::endl;
#endif
    }

//private:
    T& anything;
};


class Any
{

public: 
    template<typename T>
    Any(T& _holder)
      : holder( new Holder<T>(_holder))
    {
#ifdef DEBUG
        std::cout << "Any() creating holder:"<< holder << std::endl; 
#endif        
    }
  
    ~Any()
    {
#ifdef DEBUG
        std::cout << "~Any() deleting holder:"<< holder << std::endl; 
#endif
        delete holder;
    }

private:
    template<typename T>
    friend T any_cast(Any& any);
    
    Placeholder* holder; 
};

template<typename T>
T any_cast(Any& any)
{
#ifdef DEBUG
    std::cout << "any_cast(): anything:" << static_cast<Holder<T>*>(any.holder)->anything 
        << " at: "    
        << &(static_cast<Holder<T>*>(any.holder)->anything ) << std::endl; 
#endif
    return static_cast<Holder<T>*>(any.holder)->anything;
}

#endif

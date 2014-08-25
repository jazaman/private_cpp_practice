#ifndef _ANY_
#define _ANY_


class Placeholder 
{

public:


};

template<typename T>
class Holder: public Placeholder
{
public:
    Holder(T* _anything)
      :  anything(_anything)
    {}

  //private:
    T* anything;
};


class Any
{

public: 
    template<typename T>
    Any(T* _holder)
      : holder( new Holder<T>(_holder))
    {}

private:
    Placeholder* holder;
    
    template<typename T>
    friend T* any_cast()

};

    template<typename T>
    T* any_cast()
    {
        return static_cast<T*>(holder);
    }

#endif

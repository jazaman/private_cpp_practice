#ifndef _ANY_
#define _ANY_

class Placeholder
{

public:
    Placeholder()
    {}
    //virtual T* stored_cast();

private:

};

template<typename T>
class Storage : public Placeholder
{

public:
    //template<typename T>
    Storage(T& stored)
        : m_Storage(stored)
    {}
    
    T& get_stored()
    {
        return m_Storage;
    }

private:

    T& m_Storage;
};

class Any
{
public:
    template<typename T>
    Any(T* any)
    {
        //assert(any != 0);
        m_Any = new Storage<T>(*any);
    }

   ~Any()
    { delete m_Any;}

private:
    Placeholder* m_Any;

};



#endif

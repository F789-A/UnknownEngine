#pragma once

#include  <stdexcept>
#include <string>

template<typename T>
class Singleton
{
private:
    T* Get() const
    {
        const_cast<Singleton*>(this)->Init();
        return Data;
    }

    T* Data;

public:
    Singleton(): Data(nullptr){}

    T* operator->() 
    {
        return Get();
    }

    const T* operator->() const 
    {
        return Get();
    }

    void SetData(T* t)
    { 
        Data = t; 
    }

    void Init() 
    {
        if (Data == nullptr)
        {
            SingletonFill(*this);
        }
    }
};

template<typename T>
void SingletonFill(Singleton<T>& a)
{
    throw std::runtime_error(std::string("Cannot find implementation for interface: ") + typeid(T).name());
}


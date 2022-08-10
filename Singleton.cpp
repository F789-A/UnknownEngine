#include "Singleton.h"

template<>
void SingletonFill<SharedGraphicsResources>(Singleton<SharedGraphicsResources>& a)
{
    static SharedGraphicsResources x;
    a.SetData(&x);
}

template<>
void SingletonFill<ec::EntityManager>(Singleton<ec::EntityManager>& a)
{
    static ec::EntityManager x;
    a.SetData(&x);
}
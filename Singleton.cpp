#include "Singleton.h"

template<>
void SingletonFill<SharedGraphicsResources>(Singleton<SharedGraphicsResources>& a)
{
    static SharedGraphicsResources x;
    a.SetData(&x);
}

template<>
void SingletonFill<EntityManager>(Singleton<EntityManager>& a)
{
    static EntityManager x;
    a.SetData(&x);
}
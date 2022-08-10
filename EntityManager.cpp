#include "EntityManager.h"

template<>
void SingletonFill<ec::EntityManager>(Singleton<ec::EntityManager>& a)
{
    static ec::EntityManager x;
    a.SetData(&x);
}
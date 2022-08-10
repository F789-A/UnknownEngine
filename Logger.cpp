#include "Logger.h"

template<>
void SingletonFill<Logger>(Singleton<Logger>& a)
{
    static Logger x;
    a.SetData(&x);
}
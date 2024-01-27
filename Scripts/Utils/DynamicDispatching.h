#pragma once
#include <functional>

template<typename TypeIn, typename TypeOut, typename FuncArg1, typename FuncArg2, typename... FuncsArgs>
TypeOut DynamicDispatch(const TypeIn& A, const TypeIn& B, const std::function<TypeOut(const FuncArg1&, const FuncArg2&)>& func, 
    const FuncsArgs&... funcs)
{
    const FuncArg1* thisT1 = dynamic_cast<const FuncArg1*>(&A);
    const FuncArg2* other = dynamic_cast<const FuncArg2*>(&B);
    if (thisT1 && other)
    {
        return func(*thisT1, *other);
    }
    else if constexpr (sizeof...(FuncsArgs) > 0)
    {
        return DynamicDispatch(A, B, funcs...);
    }
    throw "type error";
}

#pragma once
#include <functional>

/*
template<typename TypeOut, typename TypeIn, typename FuncArg1, typename FuncArg2, typename... FuncsArgs>
TypeOut DynamicDispatch(const TypeIn& A, const TypeIn& B, const std::function<TypeOut(FuncArg1, FuncArg2)>& func, const FuncsArgs&... funcs)
{
    const FuncArg1* thisT1 = dynamic_cast<const FuncArg1*>(&A);
    const FuncArg2* other = dynamic_cast<const FuncArg2*>(&B);
    if (thisT1 && other)
    {
        return func(*thisT1, *other);
    }
    else if constexpr (sizeof...(FuncsArgs) > 0)
    {
        return DynamicDispatch<TypeOut, TypeIn, FuncsArgs...>(A, B, funcs);
    }
    throw "type error";
}
*/
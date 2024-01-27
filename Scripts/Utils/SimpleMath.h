#pragma once

template <typename T>
T sgn(const T& val)
{
    return (T(0) < val) - (val < T(0));
}
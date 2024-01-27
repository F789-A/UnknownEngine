#pragma once
#include <concepts>

template<class T>
concept IsComparable = requires(T a, T b)
{
	{a < b} -> std::convertible_to<bool>;
};
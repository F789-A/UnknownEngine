#pragma once

#include <vector>

class BaseComponentConteiner
{
public:
	virtual int Size()
	{
		return 0;
	}
};

template<typename T>
class ComponentConteiner: public BaseComponentConteiner
{
public:
	ComponentConteiner() = default;

	int Size() override
	{
		return Components.size();
	}

	std::vector<T*> Components;
};
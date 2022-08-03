#pragma once

template<typename T>
class Component
{
public:
	static const unsigned int Type;
	int LinkedEntity;

	int GetEntity() = delete;

	virtual ~Component() = default;
};

int generateComponentType();

template<typename T>
const unsigned int Component<T>::Type = generateComponentType();

#pragma once
#include <memory>
#include <exception>

template<typename T>
std::unique_ptr<T>& InstancePtr()
{
	static std::unique_ptr<T> instancePtr = nullptr;
	return instancePtr;
}


template<typename T, typename... Args>
void InitializeInstance(Args... args)
{
	if (InstancePtr<T>() != nullptr)
	{
		throw std::exception("Singlton already initialize");
	}
	InstancePtr<T>() = std::make_unique<T>(args...);
}

template<typename T>
T& InstanceOf()
{
	if (InstancePtr<T>() == nullptr)
	{
		throw  std::exception("Singlton was't initialize");
	}
	return *InstancePtr<T>().get();
}
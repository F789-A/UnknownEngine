#pragma once
#include "Component.h"
#include "ILoopUpdate.h"
#include <iostream>

class ComponentThatAlwaysSayHello : public ec::Component, public ILoopUpdate<UpdateType::GameLoop>
{
public:
	ComponentThatAlwaysSayHello() = default;

	ComponentThatAlwaysSayHello(std::string message)
	{
		std::cout << message << std::endl;
	}

	ComponentThatAlwaysSayHello operator=(const ComponentThatAlwaysSayHello&) = delete;
	ComponentThatAlwaysSayHello(const ComponentThatAlwaysSayHello&) = delete;
	ComponentThatAlwaysSayHello& operator=(ComponentThatAlwaysSayHello&&) = delete;
	ComponentThatAlwaysSayHello(ComponentThatAlwaysSayHello&&) = delete;

	void Update() override
	{
		std::cout << "Hello" << std::endl;
	}

	~ComponentThatAlwaysSayHello() override = default;
};
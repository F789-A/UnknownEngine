#pragma once
#include "Component.h"
#include "ILoopUpdate.h"
#include <iostream>

class ComponentThatAlwaysSayHello : public Component, public ILoopUpdate<EGameLoop>
{
public:
	ComponentThatAlwaysSayHello() = default;

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
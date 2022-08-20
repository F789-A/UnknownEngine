#pragma once
#include "Component.h"
#include "ILoopUpdate.h"
#include <iostream>

//Test class for ec, will be deleted
class ComponentThatAlwaysSayHello : public ec::Component, public ILoopUpdate<UpdateType::GameLoop>
{
public:
	ComponentThatAlwaysSayHello() = default;

	ComponentThatAlwaysSayHello(std::string message)
	{
		std::cout << message << std::endl;
	}

	void Update() override
	{
		std::cout << "Hello" << std::endl;
	}

	~ComponentThatAlwaysSayHello() override = default;
};
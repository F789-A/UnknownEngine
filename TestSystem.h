#pragma once
#include "ecs_EntityManager.h"
#include <iostream>
#include "TestComponent.h"

class ecsSystem : public ECS::SystemBase
{
public:
	static int priorityId;
	void Update() override
	{
		for (auto l = ECS::DefEcs_().entity.GetComponents<ecsComponent>(); !l.end; ++l)
		{
			//std::cout << (*l).a;
			std::cout << (*l).a;
		}
		for (auto l = ECS::DefEcs_().entity.GetComponents<ecsComponent, testComp1, testComp2>(); !l.end; ++l)
		{
			auto [ecs, t1, t2] = *l;
			ecs.a++;
			std::cout << "-----" << std::endl;
		}
	}
};

int ecsSystem::priorityId = -1;

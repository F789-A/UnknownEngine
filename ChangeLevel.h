#pragma once
#include "EcsSystem.h"
#include <string>

struct ChangeLevel : public ecs::Component<ChangeLevel>
{
	std::string NextLevel;


	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		auto& cl = em.GetComponent<ChangeLevel>(a);
		cl.NextLevel = (res["NextLevel"]);
	}
};

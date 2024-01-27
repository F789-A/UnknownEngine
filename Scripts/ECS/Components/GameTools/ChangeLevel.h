#pragma once
#include <string>

#include "ECS\EcsSystem.h"


struct ChangeLevel : public ecs::Component<ChangeLevel>
{
	static constexpr std::string_view ComponentName = "ChangeLevel";

	std::string NextLevel;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& cl = em.GetComponent<ChangeLevel>(ent);
		cl.NextLevel = (res["NextLevel"]);
	}
};

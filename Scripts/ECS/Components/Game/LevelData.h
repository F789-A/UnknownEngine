#pragma once
#include "ECS\EcsSystem.h"

struct LevelData : public ecs::Component<LevelData>
{
	static constexpr std::string_view ComponentName = "LevelData";

	int countAlien = 1;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{}
};
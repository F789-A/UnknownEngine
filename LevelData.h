#pragma once
#include "EcsSystem.h"

class LevelData : public ecs::Component<LevelData>
{
public:
	int countAlien = 1;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{}
};
#pragma once

#include "ECS\EcsSystem.h"

struct GraphDebugInfo : public ecs::Component<GraphDebugInfo>
{
	static constexpr std::string_view ComponentName = "GraphDebugInfo";

	std::vector<int> Entites;

	bool ActiveDebug = false;
	bool forceRedraw = true;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{}
};
#pragma once

#include "EcsSystem.h"

struct GraphDebugInfo : public ecs::Component<GraphDebugInfo>
{
	static constexpr std::string_view ComponentName = "GraphDebugInfo";

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{}
};
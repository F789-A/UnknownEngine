#pragma once
#include "EcsSystem.h"
#include <string>

struct Event : public ecs::Component<Event>
{
	int Id;
	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{};
};

struct Tag : public ecs::Component<Tag>
{
	std::string Name;
	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& tag = em.GetComponent<Tag>(ent);
		tag.Name = res["Name"];
	};
};
#pragma once
#include "EcsSystem.h"
#include <string>

struct Event : public ecs::Component<Event>
{
	static constexpr std::string_view ComponentName = "Event";

	int Id;
	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{};
};

struct Tag : public ecs::Component<Tag>
{
	static constexpr std::string_view ComponentName = "Tag";

	std::string Name;
	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& tag = em.GetComponent<Tag>(ent);
		tag.Name = res["Name"];
	};
};

struct NewPlacedObject : public ecs::Component<NewPlacedObject>
{
	static constexpr std::string_view ComponentName = "NewPlacedObject";

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& tag = em.GetComponent<NewPlacedObject>(ent);
	};
};
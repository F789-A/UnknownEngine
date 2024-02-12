#pragma once
#include <string>

#include "ECS\EcsSystem.h"

struct Event : public ecs::Component<Event>
{
	static constexpr std::string_view ComponentName = "Event";

	int Id;
	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{};
};

struct CreditsTableTag : public ecs::Component<CreditsTableTag>
{
	static constexpr std::string_view ComponentName = "CreditsTableTag";

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& tag = em.GetComponent<CreditsTableTag>(ent);
	};
};

struct NewPlacedObjectTag : public ecs::Component<NewPlacedObjectTag>
{
	static constexpr std::string_view ComponentName = "NewPlacedObjectTag";

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& tag = em.GetComponent<NewPlacedObjectTag>(ent);
	};
};
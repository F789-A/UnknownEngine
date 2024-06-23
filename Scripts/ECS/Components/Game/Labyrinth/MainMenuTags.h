#pragma once
#include <string>

#include "ECS\EcsSystem.h"

struct CompanyButtonTag : public ecs::Component<CompanyButtonTag>
{
	static constexpr std::string_view ComponentName = "CompanyButtonTag";

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res){}
};

struct ArcadeButtonTag : public ecs::Component<ArcadeButtonTag>
{
	static constexpr std::string_view ComponentName = "ArcadeButtonTag";

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res) {}
};

struct TutorButtonTag : public ecs::Component<TutorButtonTag>
{
	static constexpr std::string_view ComponentName = "TutorButtonTag";

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res) {}
};

struct CreditsButtonTag : public ecs::Component<CreditsButtonTag>
{
	static constexpr std::string_view ComponentName = "CreditsButtonTag";

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res){}
};

struct ExitButtonTag : public ecs::Component<ExitButtonTag>
{
	static constexpr std::string_view ComponentName = "ExitButtonTag";

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res) {}
};
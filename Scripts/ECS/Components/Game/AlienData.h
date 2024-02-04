#pragma once
#include <type_traits>
#include <glm/glm.hpp>
#include "ECS\EcsSystem.h"
#include "ECS\Components\UiComponents.h"


enum class AlienTypes : int
{
	Simpleton = 1 << 0,
	Randomer = 1 << 1,
	Coward = 1 << 2,
	//Reaction on Bound
	Bounder = 1 << 3,
	PingPonger = 1 << 4
};

AlienTypes operator|(const AlienTypes& lhs, const AlienTypes& rhs);

AlienTypes operator&(const AlienTypes& lhs, const AlienTypes& rhs);

enum class AlienState
{
	Emerge,
	Act
};

struct AlienData: public ecs::Component<AlienData>
{
	static constexpr std::string_view ComponentName = "AlienData";

	float Health;
	float Damage;
	float Support;
	AlienTypes alienType;

	Text* HealthBar;

	float TimeToChangeDir = 1;
	float Timer = 1;
	AlienState alientState;
	glm::vec2 dir;
	


	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& alien = em.GetComponent<AlienData>(ent);
		alien.Health = std::stof(res["Health"]);
		alien.Damage = std::stof(res["Damage"]);
		alien.HealthBar = &em.GetComponent<Text>(std::stoi(res["HealthBar"]));
		alien.dir = glm::normalize(glm::vec2(1, 0.3));
		alien.alienType = AlienTypes::Coward | AlienTypes::PingPonger;
	}
};
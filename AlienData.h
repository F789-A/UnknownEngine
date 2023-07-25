#pragma once
#include "EcsSystem.h"
#include <glm/glm.hpp>
#include "UiComponents.h"
#include <type_traits>

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

class AlienData: public ecs::Component<AlienData>
{
public:

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
		alien.Health = std::stoi(res["Health"]);
		alien.Damage = std::stoi(res["Damage"]);
		alien.HealthBar = &em.GetComponent<Text>(std::stoi(res["HealthBar"]));
		alien.dir = glm::normalize(glm::vec2(1, 0.3));
		alien.alienType = AlienTypes::Coward | AlienTypes::PingPonger;
	}
};

class NewPlacedAlienTag : public ecs::Component<NewPlacedAlienTag>
{
public:
	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res){}
};
#pragma once
#include "ecs_EntityManager.h"
#include <glm\\glm.hpp>
#include "UiComponents.h"

class AlienData: public ecs::Component<AlienData>
{
public:
	float Health;
	float Damage;
	Text* HealthBar;

	float TimeToDir;
	glm::vec2 dir;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& alien = em.GetComponent<AlienData>(ent);
		alien.Health = std::stoi(res["Health"]);
		alien.Damage = std::stoi(res["Damage"]);
		alien.HealthBar = &em.GetComponent<Text>(std::stoi(res["HealthBar"]));
		alien.dir = glm::vec2(1, 0);
	}
};
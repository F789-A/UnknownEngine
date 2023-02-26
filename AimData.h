#pragma once
#include <glm/vec2.hpp>
#include "ecs_EntityManager.h"
#include "SimpleTextProcessor.h"
#include "UiComponents.h"

struct AimData : public ecs::Component<AimData>
{
	float aimSensitivity;

	glm::vec2 center;
	float size;
	float Health;
	float Damage;
	float minSize = 0.1;
	float Speed = 5;

	Text* text;

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		AimData& aim = em.GetComponent<AimData>(a);
		aim.aimSensitivity = std::stof(res["aimSensitivity"]);

		aim.center = glm::vec3(0,0,0);
		aim.size = std::stof(res["size"]);
		aim.Health = stoi(res["Health"]);
		aim.Damage = stoi(res["Damage"]);

		aim.text = &em.GetComponent<Text>(std::stoi(res["text"]));
	}
};


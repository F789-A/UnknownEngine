#pragma once
#include <glm/glm.hpp>
#include "ECS\EcsSystem.h"
#include "Utils\SimpleTextProcessor.h"
#include "ECS\Components\UiComponents.h"

struct AimData : public ecs::Component<AimData>
{
	static constexpr std::string_view ComponentName = "AimData";

	float aimSensitivity;

	glm::vec2 center;
	float size;
	float Health;
	float Damage;
	float minSize = 0.1f;
	float maxSize = 0.8f;
	float Speed = 5.0f;

	bool Initialized = false;

	Text* text;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		AimData& aim = em.GetComponent<AimData>(ent);
		aim.aimSensitivity = std::stof(res["aimSensitivity"]);

		aim.center = glm::vec3(0,0,0);
		aim.size = std::stof(res["size"]);
		aim.Health = stof(res["Health"]);
		aim.Damage = stof(res["Damage"]);

		aim.text = &em.GetComponent<Text>(std::stoi(res["text"]));
	}
};

struct AimEffect : public ecs::Component<AimEffect>
{
	static constexpr std::string_view ComponentName = "AimEffect";

	struct EffectPoint
	{
		glm::vec2 TargetPos;
		glm::vec2 CurrentPos;
	};
	std::vector<EffectPoint> Lines = std::vector<EffectPoint>(40);
	float TailSpeed = 0.25f;
	float HeadSpeed = 0.30f;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{}
};


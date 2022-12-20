#pragma once
#include <glm/vec2.hpp>
#include "ecs_EntityManager.h"

struct AimData : public ECS::Component<AimData>
{
	glm::vec2 center;
	float size;
	float aspect;
	float aimSensitivity;

	int Health;
};


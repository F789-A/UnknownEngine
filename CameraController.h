#pragma once
#include "ecs_EntityManager.h"

struct CameraController : public ECS::Component<CameraController>
{
	float MouseSensitivity = 0.01;
	float Speed = 0.01;
};


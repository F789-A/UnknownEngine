#pragma once
#include "ecs_EntityManager.h"

struct CameraController : public ecs::Component<CameraController>
{
	float MouseSensitivity = 0.01;
	float Speed = 0.01;

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		CameraController& contr = em.GetComponent<CameraController>(a);
		contr.MouseSensitivity = std::stof(res["MouseSensitivity"]);
		contr.Speed = std::stof(res["Speed"]);
	}
};


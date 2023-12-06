#pragma once
#include "EcsSystem.h"

struct CameraController : public ecs::Component<CameraController>
{
	static constexpr std::string_view ComponentName = "CameraController";

	float MouseSensitivity = 0.01f;
	float Speed = 0.01f;

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		auto& contr = em.GetComponent<CameraController>(a);
		contr.MouseSensitivity = std::stof(res["MouseSensitivity"]);
		contr.Speed = std::stof(res["Speed"]);
	}
};

struct Camera2DController : public ecs::Component<Camera2DController>
{
	static constexpr std::string_view ComponentName = "Camera2DController";

	float Speed = 0.01f;

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		auto& contr = em.GetComponent<Camera2DController>(a);
		contr.Speed = std::stof(res["Speed"]);
	}
};


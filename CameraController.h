#pragma once

#include "Component.h"
#include "ILoopUpdate.h"
#include "Transform.h"
#include "Camera.h"
#include "EntityManager.h"
#include "Input/Input.h"
#include <glm/gtx/vector_angle.hpp>

class CameraController : public Component<CameraController>, public ILoopUpdate<EGameLoop>
{
	float MouseSensitivity = 0.01;
	float Speed = 0.01;

	void Update() override;
};
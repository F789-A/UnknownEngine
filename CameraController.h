#pragma once

#include "Singleton.h"
#include "Component.h"
#include "ILoopUpdate.h"
#include "Transform.h"
#include "Camera.h"
#include "Input.h"
#include <glm/gtx/vector_angle.hpp>
#include "AppTime.h"

class CameraController : public ec::Component, public ILoopUpdate<UpdateType::GameLoop>
{
private:
	float MouseSensitivity;
	float Speed;

	void Update() override;

public:
	CameraController(float mouseSensitivity = 0.01, float speed = 0.01);
	
	~CameraController() override = default;
};
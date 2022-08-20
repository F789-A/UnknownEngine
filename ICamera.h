#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ICamera
{
public:
	static ICamera* MainCamera;

	void SetCamera()
	{
		MainCamera = this;
	}

	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual float GetFOV() const = 0;
};


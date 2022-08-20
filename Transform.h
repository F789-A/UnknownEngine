#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform : public ec::Component
{
public:
	glm::vec3 Position;
	glm::quat Rotation;
	glm::vec3 Scale;

	~Transform() override = default;

	Transform(glm::vec3 pos = glm::vec3(0, 0, 0), glm::quat rot = glm::quat(1, 0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1))
	{
		Position = pos;
		Rotation = rot;
		Scale = scale;
	}

	glm::vec3 Front() const
	{
		return Rotation * glm::vec3(1, 0, 0);
	}

	glm::vec3 Right() const
	{
		return Rotation * glm::vec3(0, 0, 1);
	}

	glm::vec3 EulerAngle() const
	{
		return eulerAngles(Rotation);
	}

	void Rotate(glm::vec3 axis, float angle)
	{
		glm::quat quat;
		quat.w = cos(angle / 2);
		quat.x = axis.x * sin(angle / 2);
		quat.y = axis.y * sin(angle / 2);
		quat.z = axis.z * sin(angle / 2);
		Rotation = quat * Rotation;
	}
};
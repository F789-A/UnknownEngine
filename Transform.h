#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform : public Component
{
public:
	glm::vec3 Position;
	glm::quat Rotation;
	glm::vec3 Scale;

	Transform()
	{
		Position = glm::vec3(0, 0, 0);
		Rotation = glm::quat(1, 0, 0, 0);
		Scale = glm::vec3(1, 1, 1);
	}

	glm::vec3 Front()
	{
		return Rotation * glm::vec3(1, 0, 0);
	}

	glm::vec3 Right()
	{
		return Rotation * glm::vec3(0, 0, 1);
	}

	glm::vec3 EulerAngle()
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
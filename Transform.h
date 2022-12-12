#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <queue>

class Transform : public ec::Component
{
public:
	glm::vec3 Position;
	glm::quat Rotation;
	glm::vec3 Scale;

	glm::mat4 transformation;

	bool isLocal = false;

	Transform* parent = nullptr;
	std::vector<Transform*> childs;

	~Transform() override = default;

	Transform(glm::vec3 pos = glm::vec3(0, 0, 0), glm::quat rot = glm::quat(1, 0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1))
	{
		Position = pos;
		Rotation = rot;
		Scale = scale;
		RecalcTransformation();
	}

	void setPosition(glm::vec3 newPos)
	{
		Position = newPos;
		RecalcTransformation();
	}

	void RecalcTransformation()
	{
		transformation = glm::translate(glm::mat4(1.0f), Position) * glm::mat4_cast(Rotation) * glm::scale(glm::mat4(1.0f), Scale);
		if (isLocal)
		{
			transformation = parent->transformation * transformation;
		}
		std::queue<Transform*> ch;
		ch.push(this);
		while (!ch.empty())
		{
			Transform* cur = ch.front();
			ch.pop();

			for (auto l : cur->childs)
			{
				if (l->isLocal)
				{
					ch.push(l);
					cur->transformation = cur->parent->transformation * cur->transformation;
				}
			}
		}
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
		RecalcTransformation();
	}
};
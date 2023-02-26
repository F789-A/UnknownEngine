#pragma once
#include "EcsSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <queue>
#include "SimpleTextProcessor.h"

class Transform : public ecs::Component<Transform>
{
public:
	glm::vec3 Position = glm::vec3(0, 0, 0);
	glm::quat Rotation = glm::quat(1, 0, 0, 0);
	glm::vec3 Scale = glm::vec3(1, 1, 1);

	glm::mat4 transformation;

	bool isLocal = false;

	Transform* parent = nullptr;
	std::vector<Transform*> childs;

	~Transform() = default;

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

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		Transform& tr = em.GetComponent<Transform>(a);
		tr.Position = TextTools::ReadVec3(res["Position"]);
		tr.Scale = TextTools::ReadVec3(res["Scale"]);
		//tr.Rotation = TextTools::ReadVec3(res["Rotation"]);
	}
};
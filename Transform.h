#pragma once
#include "EcsSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <queue>
#include "SimpleTextProcessor.h"

struct Transform : public ecs::Component<Transform>
{
	static constexpr std::string_view ComponentName = "Transform";

	glm::vec3 Position = glm::vec3(0, 0, 0);
	glm::quat Rotation = glm::quat(1, 0, 0, 0);
	glm::vec3 Scale = glm::vec3(1, 1, 1);

	//glm::mat4 transformation;
	//bool isLocal = false;
	//Transform* parent = nullptr;
	//std::vector<Transform*> childs;

	static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
	{
		Transform& tr = em.GetComponent<Transform>(a);
		tr.Position = TextTools::ReadVec3(res["Position"]);
		tr.Scale = TextTools::ReadVec3(res["Scale"]);
		if (res.contains("Rotation"))
		{
			auto rot = TextTools::ReadVec3(res["Rotation"]) / 360.0f * 2.0f * std::acos(-1.0f);
			tr.Rotate(glm::vec3(1.0f, 0.0f, 0.0f), rot.x);
			tr.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), rot.y);
			tr.Rotate(glm::vec3(0.0f, 0.0f, 1.0f), rot.z);
		}
	}

	glm::vec3 Front() const;
	glm::vec3 Right() const;
	glm::vec3 EulerAngle() const;

	void Rotate(const glm::vec3& axis, float angle);

	//void RecalcTransformation();
};
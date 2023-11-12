#include "Transform.h"

glm::vec3 Transform::Front() const
{
	return Rotation * glm::vec3(1, 0, 0);
}

glm::vec3 Transform::Right() const
{
	return Rotation * glm::vec3(0, 0, 1);
}

glm::vec3 Transform::EulerAngle() const
{
	return eulerAngles(Rotation);
}

void Transform::Rotate(const glm::vec3& axis, float angle)
{
	glm::quat quat;
	quat.w = std::cos(angle / 2);
	quat.x = axis.x * std::sin(angle / 2);
	quat.y = axis.y * std::sin(angle / 2);
	quat.z = axis.z * std::sin(angle / 2);
	Rotation = quat * Rotation;
	//RecalcTransformation();
}

/*void Transform::RecalcTransformation()
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
}*/
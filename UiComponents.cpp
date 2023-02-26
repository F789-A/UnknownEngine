#include "UiComponents.h"

#include <glm/gtc/matrix_transform.hpp>

glm::vec3 ToScr(glm::vec2 pos)
{
	return glm::vec3(pos.x * 2 - 1, pos.y * 2 - 1, 0);
}

void RectTransform::UpdateMatrix()
{
	if (parent != nullptr)
	{
		globalPos = pos + parent->globalPos;
	}
	else
	{
		globalPos = pos;
	}
	std::queue<RectTransform*> que;
	for (auto l : childs)
	{
		que.push(l);
	}
	while (!que.empty())
	{
		auto& crTr = *que.front();
		que.pop();
		for (auto l : crTr.childs)
		{
			que.push(l);
		}
		crTr.globalPos = crTr.pos + crTr.parent->globalPos;
	}
}
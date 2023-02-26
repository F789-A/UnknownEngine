#include "Transform.h"

void UpdateMatrix()
{
	/*trMat = glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 0));
	trMat = glm::translate(glm::mat4(1.0f), ToScr(pos)) * trMat;
	if (parent != nullptr)
	{
		trMat = parent->trMat * trMat;
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
		crTr.trMat = glm::scale(glm::mat4(1.0f), glm::vec3(crTr.size.x, crTr.size.y, 0));
		crTr.trMat = glm::translate(glm::mat4(1.0f), ToScr(crTr.pos)) * crTr.trMat;
		crTr.trMat = crTr.parent->trMat * crTr.trMat;
	}*/
}
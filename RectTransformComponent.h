#pragma once
#include "ecs_EntityManager.h"
#include <glm/vec2.hpp>

class RectTransform : public ECS::Component<RectTransform>
{
public:
	RectTransform() : pos(0.5, 0.5), size(2, 2){};

	glm::vec2 pos;
	glm::vec2 size;
};

class Button : public ECS::Component<Button>
{
public:
	//callback void*
	//
	int identifier;
};

class Image : public ECS::Component<Image>
{
public:
	int Material;
};
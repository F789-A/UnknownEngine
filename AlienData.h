#pragma once
#include "ecs_EntityManager.h"

class AlienData: public ECS::Component<AlienData>
{
public:
	int Health;
	int Damage;
};
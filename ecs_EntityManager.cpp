#include "ecs_EntityManager.h"

ECS::EcsSystem& ECS::DefEcs_()
{
	static ECS::EcsSystem f;
	return f;
}

int ECS::generateComponentType()
{
	static int counter = 0;
	return counter++;
}
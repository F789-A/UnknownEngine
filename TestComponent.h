#pragma once
#include "ecs_EntityManager.h"

class ecsComponent : public ECS::Component<ecsComponent>
{
public:
	ecsComponent(int ent): Component<ecsComponent>(ent) {};
	int a = 1;
};

class testComp1 : public ECS::Component<testComp1>
{
public:
	testComp1(int ent) : Component<testComp1>(ent) {};
	int a = 1;
};

class testComp2 : public ECS::Component<testComp2>
{
public:
	testComp2(int ent) : Component<testComp2>(ent) {};
	int a = 1;
};
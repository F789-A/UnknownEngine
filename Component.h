#pragma once

#include "IComponent.h"
#include "EntityManagerBase.h"
#include "EntityManager.h"
#include "Entity.h"

class Component: public IComponent
{
	friend class Entity;
private:
	EntityManager* LinkedEntityManager;
	int LinkedEntity;

	void SetLinkedEntity(EntityManagerBase* manager, int entity) override
	{
		LinkedEntityManager = static_cast<EntityManager*>(manager);
		LinkedEntity = entity;
	}
public:
	Entity& GetLinkedEntity()
	{
		return LinkedEntityManager->GetEntity(LinkedEntity);
	}

	virtual ~Component() = default;
};

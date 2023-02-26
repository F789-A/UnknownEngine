#include "EcsSystem.h"

ecs::EcsSystem& ecs::DefEcs()
{
	static ecs::EcsSystem f;
	return f;
}

void ecs::EntityManager::SetActiveEntity(int ent, bool state)
{
	enabledEntity[ent] = !state;
}

std::map<std::string, std::pair<void(ecs::EntityManager::*)(int), void(*)(ecs::EntityManager& em, int, std::map<std::string, std::string>&)>>& ecs::LoadCallbacks()
{
	static std::map<std::string, std::pair<void(ecs::EntityManager::*)(int), void(*)(ecs::EntityManager& em, int, std::map<std::string, std::string>&)>> callbacks;
	return callbacks;
}

int ecs::generateComponentType()
{
	static int counter = 0;
	return counter++;
}

void ecs::EntityManager::RemoveEntity(int entity)
{
	QueueDeletion.push(entity);
}

void ecs::EntityManager::CollectGarbage()
{
	while (!QueueDeletion.empty())
	{
		freeEntity.push(QueueDeletion.front());
		for (auto l : Entityes[QueueDeletion.front()])
		{
			if (l == -1) continue;
			ComponentsConteiners[l]->Remove(QueueDeletion.front());
		}
		Entityes[QueueDeletion.front()].clear();
		QueueDeletion.pop();
	}
}

void ecs::SystemController::SetEnable(void(*syst)(EntityManager&), bool state)
{
	for (int i = 0; i < systemsPtr.size(); i++)
	{
		if (systemsPtr[i] == syst)
		{
			enabled[i] = !state;
			break;
		}
	}
}

void ecs::SystemController::Update()
{
	for (auto& l : systemsPtr)
	{
		l(ecsS->entity);
	}
	ecsS->entity.CollectGarbage();
	ecsS->EndCycle();
}

void ecs::SystemController::AddSystem(void(*syst)(EntityManager&))
{
	systemsPtr.push_back(syst);
	//numbers
}

ecs::EcsSystem::EcsSystem()
{
	system.ecsS = this;
}

void ecs::EcsSystem::EndCycle()
{
	if (tempEM)
	{
		entity = std::move(*tempEM);
		delete tempEM;
		tempEM = nullptr;
	}
}

void ecs::EcsSystem::SetEntityManager(EntityManager&& em)
{
	tempEM = new EntityManager(std::move(em));
}
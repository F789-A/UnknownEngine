#include "EcsSystem.h"

ecs::EcsSystem& ecs::EcsSystem::GetInstance()
{
	static ecs::EcsSystem f;
	return f;
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
	if (Entities.size() <= entity || Entities[entity].empty())
	{
		throw "incorrect RemoveEntity";
	}

	freeEntities.push(entity);
	for (auto l : Entities[entity])
	{
		if (l == -1) continue;
		ComponentsConteiners[l]->SoftRemove(entity);
	}
	Entities[entity].clear();
}

void ecs::EntityManager::CollectGarbage()
{
	while (!QueueDeletionComponent.empty())
	{
		const auto& compToDelete = QueueDeletionComponent.front();
		if (Entities[compToDelete.second].contains(compToDelete.first))
		{
			ComponentsConteiners[compToDelete.first]->Remove(compToDelete.second);
		}
		QueueDeletionComponent.pop();
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

void ecs::CreateComponent(ecs::EntityManager& em, const std::string& componentName, int entity)
{
	(em.*ecs::LoadCallbacks()[componentName].first)(entity);
}

void ecs::LoadComponent(const std::string& componentName, ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
{
	(ecs::LoadCallbacks()[componentName].second)(em, ent, res);
}
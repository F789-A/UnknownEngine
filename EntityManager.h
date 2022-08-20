#pragma once

#include "EntityManagerBase.h"
#include "Entity.h"

#include <vector>
#include <map>
#include <memory>
#include "Singleton.h"

namespace ec
{
	class EntityManager : public EntityManagerBase
	{
	public:

		EntityManager() {}

		EntityManager(const EntityManager&) = delete;
		EntityManager& operator=(const EntityManager&) = delete;
		EntityManager(EntityManager&& other) = delete;
		EntityManager& operator=(EntityManager&& other) = delete;

		Entity& AddEntity()
		{
			if (FreeEntity.size() == 0)
			{
				int count = Entities.size();
				Entities.push_back(Entity(*this, count));
				return Entities.back();
			}
			else
			{
				int res = FreeEntity.back();
				FreeEntity.pop_back();
				Entities[res] = Entity(*this, res);
				return Entities[res];
			}
		}

		void RemoveEntity(int entity)
		{
			FreeEntity.push_back(entity);
			Entities[entity] = Entity(*this, entity);
		}

		Entity& GetEntity(int entity)
		{
			return Entities[entity];
		}

		const Entity& GetEntity(int entity) const
		{
			return Entities[entity];
		}

		int EntityCount() const
		{
			return Entities.size();
		}

		~EntityManager() = default;

	private:
		std::vector<Entity> Entities;
		std::vector<int> FreeEntity; // заменить на двусвязный список
	};
}

template<>
void SingletonFill<ec::EntityManager>(Singleton<ec::EntityManager>& a);
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

		//TODO удалить копирование

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

		int EntityCount()
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
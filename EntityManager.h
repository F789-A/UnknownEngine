#pragma once

#include "ComponentConteiner.h"
#include "Entity.h"
#include <vector>
#include <map>
#include <memory>

class EntityManager
{
public:
	static const int MaxComponentCount = 128;

	EntityManager()
	{
		ComponentConteiners = std::vector<std::unique_ptr<BaseComponentConteiner>>(MaxComponentCount);
	}

	static EntityManager& Manager()
	{
		static EntityManager manager;
		return manager;
	}

	//index with type - is universal identifier
	template<typename T>
	void AddComponent(Entity entity) 
	{
		if (ComponentConteiners[T::Type] == nullptr)
		{
			ComponentConteiners[T::Type] = std::make_unique<ComponentConteiner<T>>();
		}
		std::vector<T*>& conteiner = GetConteiner<T>();
		T* newComp = new T();
		newComp->LinkedEntity = entity;
		conteiner.push_back(newComp);
		int index = conteiner.size()-1;
		EntityToComponent[entity].push_back({T::Type, index});
		ComponentToEntity[{T::Type, index}] = entity;
	}

	//TODO it works only with single component of all type
	template<typename T>
	void RemoveComponent(Entity entity)
	{
		/*int index = 0;
		bool finded = false;
		for (int i = 0; i < EntityToComponent[entity].size(); i++)
		{
			if (EntityToComponent[entity].first == T.Type)
			{
				index = EntityToComponent[entity].second;
				EntityToComponent[entity].erase(i);
				finded = true;
				break;
			}
		}
		if (!finded)
		{
			throw "---";
		}
		ComponentToEntity.erase({ T.Type, index });
		GetConteiner(T.Type).errase(index);*/
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		bool finded = false;
		int index = 0;
		for (int i = 0; i < EntityToComponent[entity].size(); i++)
		{
			if (EntityToComponent[entity][i].first == T::Type)
			{
				index = EntityToComponent[entity][i].second;
				finded = true;
				break;
			}
		}
		if (!finded)
		{
			throw "потом код придумаю";
		}
		std::vector<T*>& a = GetConteiner<T>();
		return *(GetConteiner<T>()[index]);
	}

	Entity CreateEntity()
	{
		if (FreeEntity.size() == 0)
		{
			EntityToComponent.push_back(std::vector<std::pair<int, int>>());
			return EntityToComponent.size() - 1;
		}
		else
		{
			int res = FreeEntity.front();
			FreeEntity.pop_back();
			return res;
		}
	}

	void RemoveEntity(Entity entity)
	{
		/*for (auto l : EntityToComponent[entity])
		{
			GetConteiner(l->first).errase(l->second);
		}
		EntityToComponent[entity].clear();
		ComponentToEntity.erase({ T.Type, index });*/
	}

	template<typename T>
	int GetEntity(T& component)
	{
		return component.LinkedEntity;
	}

	template<typename T>
	std::vector<T*>& GetConteiner()
	{
		return (static_cast<ComponentConteiner<T>*>(ComponentConteiners[T::Type].get()))->Components;
	}

private:
	std::vector<std::unique_ptr<BaseComponentConteiner>> ComponentConteiners;
	std::vector<Entity> FreeEntity; // заменить на двусвязный список
	std::vector<std::vector<std::pair<int, int>>> EntityToComponent;
	std::map<std::pair<int, int>, Entity> ComponentToEntity;
};
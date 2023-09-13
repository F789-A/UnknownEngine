#pragma once
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <bitset>
#include <set>
#include <memory>
#include "SortedByValueMap.h"
#include "ConceptLib.h"
#include <iostream>
#include <functional>

//TODO: TAGS, EVENTS

namespace ecs
{
	template<typename T>
	class ComponentConteiner;
	class EntityManager;

	template<typename T>
	class Component
	{
		friend EntityManager;
		friend ComponentConteiner<T>;
	public:
		int entity;
		static const int type;

	public:
		Component() = default;
		virtual ~Component() = default;
	};

	int generateComponentType();

	class BaseComponentConteiner
	{
	public:
		virtual void Remove(int entity) = 0;
		virtual int Count() = 0;
		virtual void Add(int ent) = 0;
	};

	template<typename T>
	class ComponentConteiner : public BaseComponentConteiner
	{
	public:
		using iterator = std::unordered_map<int, T>::iterator;
		
		iterator begin()
		{
			return components.begin();
		}

		iterator end()
		{
			return components.end();
		}

		void Remove(int entity)
		{
			components.erase(entity);
		}
		int Count()
		{
			return components.size();
		}

		void Add(int entity)
		{
			components.emplace(entity, T());
			components[entity].entity = entity;
		}

		T& Get(int ent)
		{
			return components.at(ent);
		}

		std::unordered_map<int, T> components;
	};

	class EntityManager
	{
	public:

		//Iterator
		template<typename T1, typename... Ts>
		class ComponentIterator
		{
			friend class EntityManager;
		public:
			bool end();
			bool operator==(const ComponentIterator& b);
			ComponentIterator& operator++();
			std::tuple<T1&, Ts&...> operator*();

		private:
			EntityManager& em;
			typename ComponentConteiner<T1>::iterator it_end;
			typename ComponentConteiner<T1>::iterator it;

			ComponentIterator(EntityManager& em);
		};

		//Entityes
		template<typename... Ts> 
		int AddEntity();
		template<typename T>
		int GetEntity(const Component<T>& comp);
		void RemoveEntity(int entity);

		void CollectGarbage();
		//Components

		template<typename T>
		void AddComponent(int entity);
		template<typename Arg1, typename Arg2, typename... Args>
		void AddComponent(int entity);

		template<typename T>
		T& GetComponent(int entity);
		template<typename T, typename R>
		T& GetComponent(R& comp);

		void SetActiveEntity(int ent, bool state);

		template<typename Arg1, typename... Args>
		ComponentIterator<Arg1, Args...> GetComponents();

		template<typename T>
		void RemoveComponent(int entity);

		template<typename Arg1, typename Arg2, typename... Args>
		bool CheckComponents(int ent);
		template<typename Arg1>
		bool CheckComponents(int ent);

		std::vector<std::function<void(int, int)>> onComponentRemoveCollbacks;
	private:
		template<typename T>
		void Register();

		template<typename T> 
		ComponentConteiner<T>& GetConteiner();

		std::vector<std::unique_ptr<BaseComponentConteiner>> ComponentsConteiners;
		std::vector<std::set<int>> Entityes;
		std::queue<int> QueueDeletion;
		std::queue<std::pair<int, int>> QueueDeletionComponent;
		std::queue<int> freeEntity;
		std::bitset<1000> enabledEntity;
		
	};

	class EcsSystem;

	class SystemController
	{
	public:
		void Update();

		void AddSystem(void(*syst)(EntityManager&));

		void SetEnable(void(*syst)(EntityManager&), bool state);

		EcsSystem* ecsS = nullptr;
		std::bitset<100> enabled;
		std::vector<void(*)(EntityManager&)> systemsPtr;
	};

	class EcsSystem
	{
	public:
		SystemController system;
		EntityManager entity;

		EcsSystem();
		void EndCycle();
		void SetEntityManager(EntityManager&& em);

	private:
		EntityManager* tempEM;
	};

	EcsSystem& DefEcs();

	//Serialization
	std::map<std::string, std::pair<void(ecs::EntityManager::*)(int), void(*)(EntityManager& em, int, std::map<std::string, std::string>&)>>& LoadCallbacks();

	template<typename T>
	int RegisterComponent()
	{
		std::string raw = typeid(T).name(); // Можно заменить на статичное поле внутри каждого компонента
		std::string name = raw.substr(raw.find(" ") + 1, raw.length());
		LoadCallbacks()[name] = { &ecs::EntityManager::AddComponent<T>, &T::Load };

		return generateComponentType();
	}

	template<typename T>
	const int Component<T>::type = RegisterComponent<T>();
};

//Implemetation

//EntityManager
template<typename... Ts>
int ecs::EntityManager::AddEntity()
{
	int ent = -1;
	if (!freeEntity.empty())
	{
		ent = freeEntity.front();
		freeEntity.pop();
		Entityes[ent] = { -1 };
	}
	else
	{
		Entityes.push_back({ -1 });
		ent = Entityes.size() - 1;
	}
	if constexpr (sizeof...(Ts) > 0)
	{
		AddComponent<Ts...>(ent);
	}
	return ent;
}
template<typename T>
int ecs::EntityManager::GetEntity(const Component<T>& comp)
{
	return comp.entity;
}
template<typename T>
void ecs::EntityManager::AddComponent(int entity)
{
	if (ComponentsConteiners.size() <= T::type || ComponentsConteiners[T::type] == nullptr)
	{
		Register<T>();
	}
	ComponentsConteiners[T::type]->Add(entity);
	Entityes[entity].insert(T::type);
}
template<typename Arg1, typename Arg2, typename... Args>
void ecs::EntityManager::AddComponent(int entity)
{
	AddComponent<Arg1>(entity);
	AddComponent<Arg2, Args...>(entity);
}
template<typename T>
T& ecs::EntityManager::GetComponent(int entity)
{
	return GetConteiner<T>().Get(entity);
}
template<typename T, typename R>
T& ecs::EntityManager::GetComponent(R& comp)
{
	return GetConteiner<T>().Get(GetEntity(comp));
}
template<typename Arg1, typename Arg2, typename... Args>
bool ecs::EntityManager::CheckComponents(int ent)
{
	if (!CheckComponents<Arg1>(ent))
	{
		return false;
	}
	return CheckComponents<Arg2, Args...>(ent);
}
template<typename Arg1>
bool ecs::EntityManager::CheckComponents(int ent)
{
	if (Entityes[ent].find(Arg1::type) != Entityes[ent].end())
	{
		return true;
	}
	return false;
}
template<typename Arg1, typename... Args>
ecs::EntityManager::ComponentIterator<Arg1, Args...> ecs::EntityManager::GetComponents()
{
	if (Arg1::type >= ComponentsConteiners.size() || ComponentsConteiners[Arg1::type] == nullptr)
	{
		Register<Arg1>();
	}
	return ComponentIterator<Arg1, Args...>(*this);
}
template<typename T>
void ecs::EntityManager::RemoveComponent(int entity)
{
	QueueDeletionComponent.push({ T::type, entity });
}

template<typename T>
void ecs::EntityManager::Register()
{
	if (ComponentsConteiners.size() <= T::type)
	{
		ComponentsConteiners.resize(T::type + 1);
	}
	ComponentsConteiners[T::type] = std::make_unique<ComponentConteiner<T>>();
}
template<typename T>
ecs::ComponentConteiner<T>& ecs::EntityManager::GetConteiner()
{
	if (T::type >= ComponentsConteiners.size() || ComponentsConteiners[T::type] == nullptr)
	{
		Register<T>();
	}
	return *static_cast<ComponentConteiner<T>*>(ComponentsConteiners[T::type].get());
}

//Iterator
template<typename T1, typename... Ts>
bool ecs::EntityManager::ComponentIterator<T1, Ts...>::end()
{
	return it == it_end;
}
template<typename T1, typename... Ts>
bool ecs::EntityManager::ComponentIterator<T1, Ts...>::operator==(const ComponentIterator& b)
{
	return it == b.it;
}
template<typename T1, typename... Ts>
ecs::EntityManager::ComponentIterator<T1, Ts...>& ecs::EntityManager::ComponentIterator<T1, Ts...>::operator++()
{
	++it;
	if constexpr (sizeof...(Ts) > 0)
	{
		while (it != it_end && !em.CheckComponents<Ts...>(it->first))
		{
			++it;
		}
	}
	return *this;
}
template<typename T1, typename... Ts>
std::tuple<T1&, Ts&...> ecs::EntityManager::ComponentIterator<T1, Ts...>::operator*()
{
	return std::tie(em.GetComponent<T1>(it->first), em.GetComponent<Ts>(it->first)...);
}
template<typename T1, typename... Ts>
ecs::EntityManager::ComponentIterator<T1, Ts...>::ComponentIterator(EntityManager& em_) :
	em(em_),
	it(em_.GetConteiner<T1>().begin()),
	it_end(em.GetConteiner<T1>().end())
{
	if constexpr (sizeof...(Ts) > 0)
	{
		while (it != it_end && !em.CheckComponents<Ts...>(it->first))
		{
			++it;
		}
	}

};
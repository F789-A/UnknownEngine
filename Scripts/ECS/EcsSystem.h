#pragma once
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <bitset>
#include <set>
#include <memory>
#include <functional>

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

		int entity;
		static const int type;

	public:
		Component() = default;
		virtual ~Component() = default;
	};

	//namespace
	//{
	int generateComponentType();
	template<typename T>
	int RegisterComponent();
	//}
	template<typename T>
	const int Component<T>::type = RegisterComponent<T>();

	class BaseComponentConteiner
	{
	public:
		virtual void Add(int ent) = 0;
		virtual void Remove(int entity) = 0;
		virtual void SoftRemove(int entity) = 0;
	};

	template<typename T>
	class ComponentConteiner : public BaseComponentConteiner
	{
	public:
		using iterator = std::unordered_map<int, T>::iterator;
		
		iterator begin();
		iterator end();

		void Add(int entity);
		void Remove(int entity);
		void SoftRemove(int entity);
		T& Get(int ent);
		
	private:
		std::unordered_map<int, T> components;
	};

	//Нельзя делать AddComponent для компонента, который в данный момент перебирается
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
			using BaseComponentIterator = ComponentConteiner<T1>::iterator;

			EntityManager& em;
			BaseComponentIterator it_end;
			BaseComponentIterator it;

			ComponentIterator(EntityManager& em);
		};

		//Entities
		template<typename... Ts> 
		int AddEntity();
		template<typename T>
		int GetEntity(const Component<T>& comp);
		void RemoveEntity(int entity);

		//Components
		template<typename T>
		void AddComponent(int entity);
		template<typename Arg1, typename Arg2, typename... Args>
		void AddComponent(int entity);

		template<typename T>
		T& GetComponent(int entity);
		template<typename T, typename R>
		T& GetComponent(R& comp);

		template<typename Arg1, typename... Args>
		ComponentIterator<Arg1, Args...> GetComponents();

		template<typename T>
		void RemoveComponent(int entity);

		//other
		void CollectGarbage();

	//not public:
		template<typename Arg1, typename Arg2, typename... Args>
		bool CheckComponents(int ent);
		template<typename Arg1>
		bool CheckComponents(int ent);

	private:
		template<typename T>
		void Register();

		template<typename T> 
		ComponentConteiner<T>& GetConteiner();

		std::vector<std::unique_ptr<BaseComponentConteiner>> ComponentsConteiners;
		std::vector<std::set<int>> Entities;

		std::queue<std::pair<int, int>> QueueDeletionComponent;
		std::queue<int> freeEntities;
	};

	class EcsSystem;

	class SystemController
	{
	public:
		void Update();

		void AddSystem(void(*syst)(EntityManager&));

		EcsSystem* ecsS = nullptr;
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

		static EcsSystem& GetInstance();

	private:
		EntityManager* tempEM;
	};

	//utils
	void CreateComponent(ecs::EntityManager& em, const std::string& componentName, int entity);
	void LoadComponent(const std::string& componentName, ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res);

	//namespace
	//{
	std::map<std::string, std::pair<void(ecs::EntityManager::*)(int), void(*)(EntityManager& em, int, std::map<std::string, std::string>&)>>& LoadCallbacks();
	//}
};

//IMPLEMENTATION

//Component
template<typename T>
int ecs::RegisterComponent<T>()
{
	LoadCallbacks()[std::string(T::ComponentName)] = { &ecs::EntityManager::AddComponent<T>, &T::Load };
	return generateComponentType();
}

//ComponentConteiner
template<typename T>
ecs::ComponentConteiner<T>::iterator ecs::ComponentConteiner<T>::begin()
{
	return components.begin();
}
template<typename T>
ecs::ComponentConteiner<T>::iterator ecs::ComponentConteiner<T>::end()
{
	return components.end();
}
template<typename T>
void ecs::ComponentConteiner<T>::Remove(int entity)
{
	components.erase(entity);
}
template<typename T>
void ecs::ComponentConteiner<T>::SoftRemove(int entity)
{
	components.at(entity).entity = -1;
}
template<typename T>
void ecs::ComponentConteiner<T>::Add(int entity)
{
	components.insert_or_assign(entity, T());
	components[entity].entity = entity;
}
template<typename T>
T& ecs::ComponentConteiner<T>::Get(int ent)
{
	return components.at(ent);
}

//EntityManager
template<typename... Ts>
int ecs::EntityManager::AddEntity()
{
	int ent = -1;
	if (!freeEntities.empty())
	{
		ent = freeEntities.front();
		freeEntities.pop();
		Entities[ent] = { -1 };
	}
	else
	{
		Entities.push_back({ -1 });
		ent = Entities.size() - 1;
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
	if (Entities[entity].size() == 0 || Entities[entity].contains(T::type))
	{
		throw "incorrect AddComponent";
	}

	if (ComponentsConteiners.size() <= T::type || ComponentsConteiners[T::type] == nullptr)
	{
		Register<T>();
	}
	ComponentsConteiners[T::type]->Add(entity);
	Entities[entity].insert(T::type);
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
	if (!Entities[entity].contains(T::type))
	{
		throw "incorrect GetComponent";
	}
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
	if (Entities[ent].find(Arg1::type) != Entities[ent].end())
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
	if (!Entities[entity].contains(T::type))
	{
		throw "incorrect RemoveComponent";
	}
	ComponentsConteiners[T::type]->SoftRemove(entity);
	Entities[entity].erase(T::type);

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
	while (it != it_end && !em.CheckComponents<T1, Ts...>(it->first))
	{
		++it;
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
	while (it != it_end && !em.CheckComponents<T1, Ts...>(it->first))
	{
		++it;
	}

};
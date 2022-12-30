#pragma once
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <bitset>
#include <set>
#include <memory>

namespace ECS
{
	class EntityManager;

	template<typename T>
	class Component
	{
		friend EntityManager;
	public:
		int entity;
		static const int type;

	public:
		Component() = default;
	};

	int generateComponentType();

	template<typename T>
	const int Component<T>::type = generateComponentType();

	class BaseComponentConteiner
	{
	public:
		virtual void Remove(int entity) = 0;
		virtual int Count() = 0;
	};

	template<typename T>
	struct ComponentConteiner : public BaseComponentConteiner
	{
	public:
		void Remove(int entity)
		{
			components.erase(entity);
		}
		int Count()
		{
			return 0;
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
			bool end()
			{
				return it == it_end;
			}

			void operator++()
			{
				it++;
				if constexpr(sizeof...(Ts) > 0)
				{
					while (it != it_end && !checkReg<Ts...>(it->first))
					{
						it++;
					}
				}
			}
			
			std::tuple<T1&, Ts&...> operator*()
			{
				return std::tie(em.GetComponent<T1>(it->first), em.GetComponent<Ts>(it->first)...);
			}

		private:
			EntityManager& em;
			typename std::unordered_map<int, T1>::iterator it_end;
			typename std::unordered_map<int, T1>::iterator it;

			ComponentIterator(EntityManager& em_): em(em_), it(em_.GetConteiner<T1>().components.begin()),
				it_end(em.GetConteiner<T1>().components.end())
			{}

			template<typename Arg1, typename Arg2, typename... Args>
			bool checkReg(int ent)
			{
				if (!checkReg<Arg1>(ent))
				{
					return false;
				}
				return checkReg<Arg1, Args...>(ent);
			}

			template<typename Arg1>
			bool checkReg(int ent)
			{
				if (em.Entityes[ent].find(Arg1::type) != em.Entityes[ent].end())
				{
					return true;
				}
				return false;
			}
		};

		//Entityes
		template<typename... Ts>
		int AddEntity()
		{
			int ent = -1;
			if (!freeEntity.empty())
			{
				ent = freeEntity.front();
				freeEntity.pop();
				Entityes[ent] = {-1};
			}
			else
			{
				Entityes.push_back({-1});
				ent = Entityes.size() - 1;
			}
			if constexpr(sizeof...(Ts) > 0)
			{
				AddComponent<Ts...>(ent);
			}
			return ent;
		}


		template<typename T>
		int GetEntity(const Component<T>& comp) 
		{
			return comp.entity;
		}

		void RemoveEntity(int entity)
		{
			freeEntity.push(entity);
			for (auto l : Entityes[entity])
			{
				if (l == -1) continue;
				ComponentsConteiners[l]->Remove(entity);
			}
			Entityes[entity].clear();
		}

		//Components

		template<typename T>
		void AddComponent(int entity)
		{
			if (ComponentsConteiners.size() <= T::type || ComponentsConteiners[T::type] == nullptr)
			{
				regist<T>();
			}
			GetConteiner<T>().components.emplace(entity, T());
			GetConteiner<T>().components[entity].entity = entity;
			Entityes[entity].insert(T::type);
		}

		template<typename Arg1, typename Arg2, typename... Args>
		void AddComponent(int entity)
		{
			AddComponent<Arg1>(entity);
			AddComponent<Arg2, Args...>(entity);
		}

		template<typename T>
		T& GetComponent(int entity)
		{
			return GetConteiner<T>().components.at(entity);
		}

		template<typename Arg1, typename... Args>
		ComponentIterator<Arg1, Args...> GetComponents()
		{
			if (Arg1::type >= ComponentsConteiners.size() || ComponentsConteiners[Arg1::type] == nullptr)
			{
				regist<Arg1>();
			}
			return ComponentIterator<Arg1, Args...>(*this);
		}

		template<typename T>
		void RemoveComponent(int entity)
		{
			ComponentsConteiners[T::Type]->Remove(entity);
		}

	private:
		template<typename T>
		void regist()
		{
			if (ComponentsConteiners.size() <= T::type)
			{
				ComponentsConteiners.resize(T::type + 1);
			}
			ComponentsConteiners[T::type] = std::make_unique<ComponentConteiner<T>>();
		}

		template<typename T> 
		ComponentConteiner<T>& GetConteiner()
		{
			if (T::type >= ComponentsConteiners.size() || ComponentsConteiners[T::type] == nullptr)
			{
				regist<T>();
			}
			return *static_cast<ComponentConteiner<T>*>(ComponentsConteiners[T::type].get());
		}

		std::vector<std::unique_ptr<BaseComponentConteiner>> ComponentsConteiners;
		std::vector<std::set<int>> Entityes;

		std::queue<int> freeEntity;
	};

	class SystemController
	{
	public:
		void Enable(int priority)
		{
	
		}

		void Disable(int priority)
		{
			
		}

		void Update()
		{
			for (auto& l : systemsPtr)
			{
				l();
			}
		}

		std::map<std::string, int> numbers;
		std::vector<bool> enabled;
		std::vector<void(*)()> systemsPtr;
	};

	class EcsSystem
	{
	public:
		SystemController system;
		EntityManager entity;
	};

	EcsSystem& DefEcs_();
}
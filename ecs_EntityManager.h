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
	template<typename T>
	class Component
	{
	public:
		using typeName = T;
		Component(int ent) : entity(ent) {};
		const int entity;
		static const int type;
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
		template<typename T>
		class ComponentIterator
		{
			typename std::unordered_map<int, T>::iterator it_end;
			typename std::unordered_map<int, T>::iterator it;
		public:
			bool end = false;

			ComponentIterator(EntityManager& em)
			{
				it = em.GetConteiner<T>().components.begin();
				it_end = em.GetConteiner<T>().components.end();
			}

			bool operator!=(const ComponentIterator& b)
			{
				return it != b.it && (end != b.end);
			}

			void operator++()
			{
				it++;
				end = (it == it_end) ? true : false;
			}

			T& operator*()
			{
				return it->second;
			}
		};

		template<typename T1, typename T2, typename... Ts>
		class ComponentIterator_
		{
			EntityManager& em;
			typename std::unordered_map<int, T1>::iterator it_end;
			typename std::unordered_map<int, T1>::iterator it;
		public:
			bool end = false;

			ComponentIterator_(EntityManager& em_): em(em_)
			{
				it = em.GetConteiner<T1>().components.begin();
				it_end = em.GetConteiner<T1>().components.end();
			}

			bool operator!=(const ComponentIterator_& b)
			{
				return it != b.it && (end != b.end);
			}

			template<typename Arg1, typename Arg2, typename... Args>
			bool foo(int ent)
			{
				return true;
			}

			void operator++()
			{
				it++;
				if (it == it_end)
				{
					end = true;
					return;
				}
				while (!checkReg<T2, Ts...>(it->first))
				{
					it++;
					if (it == it_end)
					{
						end = true;
						break;
					}
				}
				end = (it == it_end) ? true : false;
			}

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

			std::tuple<T1&, T2&, Ts&...> operator*()
			{
				return std::tie(em.GetComponent<T1>(it->first), em.GetComponent<T2>(it->first),
					em.GetComponent<Ts>(it->first)...);
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
			if (sizeof...(Ts) > 0)
			{
				AddComponent<Ts...>(ent);
			}
			return ent;
		}


		template<typename T>
		int GetEntity(Component<T>& comp)
		{
			return comp->Entity;
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
			GetConteiner<T>().components.emplace(entity, T(entity));
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

		template<typename Arg1, typename Arg2, typename... Args>
		ComponentIterator_<Arg1, Arg2, Args...> GetComponents()
		{
			return ComponentIterator_<Arg1, Arg2, Args...>(*this);
		}

		template<typename T>
		ComponentIterator<T> GetComponents()
		{
			return ComponentIterator<T>(*this);
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

	class SystemBase
	{
	public:
		virtual void Update() = 0;
	};

	class SystemController
	{
	public:
		template<typename T>
		void Register(int priority)
		{
			if (systems.size() < priority+1)
			{
				systems.resize(priority+1);
			}
			systems[priority] = std::make_unique<T>();
			T::priorityId = priority;
		}

		template<typename T>
		void Delete()
		{
			systems[T::priorityId] = nullptr;
			T::priorityId = -1;
		}

		void Update()
		{
			for (auto& l : systems)
			{
				l->Update();
			}
		}

	private:


		std::vector<std::unique_ptr<SystemBase>> systems;
	};

	class EcsSystem
	{
	public:
		SystemController system;
		EntityManager entity;
	};

	//EcsSystem DefEcs;
	EcsSystem& DefEcs_();
}
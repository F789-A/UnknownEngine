#pragma once

#include "IComponent.h"
#include "EntityManagerBase.h"

#include <vector>
#include <memory>
#include <exception>


namespace ec
{
	class Entity
	{
	private:
		EntityManagerBase* LinkedEntityManager;

		std::vector<std::unique_ptr<IComponent>> Components;
	public:
		int Id;

		
		Entity() = delete;

		Entity(EntityManagerBase& manager, int id) : LinkedEntityManager(&manager), Id(id)
		{}

		Entity(EntityManagerBase& manager, int id, int count) : LinkedEntityManager(&manager), Id(id)
		{
			Components.reserve(count);
		}

		template<typename T>
		T& GetComponent()
		{
			for (int i = 0; i < Components.size(); i++)
			{
				if (typeid(T) == typeid(*Components[i]))
				{
					return *static_cast<T*>(Components[i].get());
				}
			}
			throw std::exception("Component doesn't exists");
		}

		template<typename T>
		const T& GetComponent() const
		{
			for (int i = 0; i < Components.size(); i++)
			{
				if (typeid(T) == typeid(*Components[i]))
				{
					return *static_cast<T*>(Components[i].get());
				}
			}
			throw "Потом";
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) // для удобства
		{
			Components.push_back(std::make_unique<T>(std::forward<Args>(args)...));
			Components.back()->SetLinkedEntity(LinkedEntityManager, Id);
			return *static_cast<T*>(Components.back().get());
		}

		template<typename T>
		void RemoveComponent()
		{
			for (int i = 0; i < Components.size(); i++)
			{
				Components.erase();
			}
		}
	};
}
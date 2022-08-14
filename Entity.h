#pragma once

#include "IComponent.h"
#include "EntityManagerBase.h"

#include <vector>
#include <memory>


namespace ec
{
	class Entity
	{
	private:
		EntityManagerBase* LinkedEntityManager;

		std::vector<std::unique_ptr<IComponent>> Components;
	public:
		int Id;

		//добавить шаблонный конструктор с перечислением типов компонентов
		Entity() = delete;

		Entity(EntityManagerBase& manager, int id) : LinkedEntityManager(&manager), Id(id)
		{}

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
			throw "ѕотом";

		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
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
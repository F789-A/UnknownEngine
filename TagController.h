#pragma once
#include "EcsSystem.h"

class TagController
{
public:
	TagController() = default;

	void Bind(ecs::EntityManager& em)
	{
		entityManager = &em;
		auto onRemove
		{
			[&tags = tags, &entToTag = entToTag](int ent, int type)
			{
				if (entToTag.find({ent, type}) != entToTag.end())
				{
					std::string tag = entToTag.at({ ent, type });
					tags.erase({ tag, type });
					entToTag.erase({ ent, type });
				}
			}
		};
		linkedCollback = em.onComponentRemoveCollbacks.size();
		em.onComponentRemoveCollbacks.push_back(onRemove);
	}

	~TagController()
	{
		if (entityManager != nullptr)
		{
			entityManager->onComponentRemoveCollbacks.erase(std::next(entityManager->onComponentRemoveCollbacks.begin(), linkedCollback));
		}
	}

	template<typename T>
	void AddTagToComponent(const std::string& tag, T& comp)
	{
		add(entityManager.GetEntity(comp), T::type, tag);
	}

	void AddTagToEntity(const std::string& tag, int ent)
	{
		add(ent, -1, tag);
	}

	template<typename T>
	T& GetComponentWithTag(const std::string& tag)
	{
		return entityManager->GetComponent<T>(tags.at({ tag, T::type }));
	}

	int GetEntityWithTag(const std::string& tag)
	{
		return tags.at({tag, -1});
	}

	void RemoveTagOnEntity(const std::string& tag)
	{
		remove(tag, -1);
	}

	template<typename T>
	void RemoveTagOnComponent(const std::string& tag)
	{
		remove(tag, T::type);
	}

private:

	inline void add(int ent, int type, const std::string& tag)
	{
		if (tags.find({ tag, type }) != tags.end())
		{
			throw "bad";
		}
		entToTag[{ent, type}] = tag;
		tags[{tag, type}] = ent;
	}

	inline void remove(const std::string& tag, int type)
	{
		int ent = tags.at({ tag, type });
		tags.erase({ tag, type });
		entToTag.erase({ ent, type });
	}

	inline void remove(int ent, int type)
	{
		std::string tag = entToTag.at({ ent, type });
		tags.erase({ tag, type });
		entToTag.erase({ ent, type });
	}

	std::map<std::pair<std::string, int>, int> tags;
	std::map<std::pair<int, int>, std::string> entToTag;
	int linkedCollback = -1;
	ecs::EntityManager* entityManager = nullptr;
};
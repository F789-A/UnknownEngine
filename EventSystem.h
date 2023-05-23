#pragma once
#include "TagController.h"
#include "EcsSystem.h"
#include <string>

class EventSystem
{
	template<typename T>
	void AddEntityEvent(int ent);
	
	template<typename T>
	void AddEvent(const std::string& name);

	template<typename T>
	T& GetEvent(const std::string& name);

	void Bind(TagController*, ecs::EntityManager*);

private:
	TagController* tagController;
	ecs::EntityManager* entityManager;
};

template<typename T>
void EventSystem::AddEntityEvent(int ent)
{
	entityManager->AddComponent<T>(ent);
}

template<typename T>
void EventSystem::AddEvent(const std::string& name)
{
	int ent = entityManager->AddEntity<T>();
	tagController->AddTagToComponent(name, entityManager->GetComponent<T>(ent));
}

template<typename T>
T& EventSystem::GetEvent(const std::string& name)
{
	return tagController->GetComponentWithTag<T>(name);
}
#include "EventSystem.h"

void EventSystem::Bind(TagController* tagContr, ecs::EntityManager* entManag)
{
	tagController = tagContr;
	entityManager = entManag;
}
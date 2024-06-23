#include "ECS\Systems\Systems.h"
#include "ECS\Components\Events.h"
#include "ECS\Components\UiComponents.h"
#include "Core\WindowApp.h"
#include "SerializationSystem\EntityLoader.h"

namespace AsteroidHunter
{
	void MenuEvent(ecs::EntityManager& em)
	{
		for (auto l = em.GetComponents<Event>(); !l.end(); ++l)
		{
			auto [ev] = *l;
			if (ev.Id == 1)
			{
				ecs::EntityManager tmpEm;
				SerializationSystem::LoadEntity(tmpEm, "Scenes\\AsteroidHunter\\Level1.txt");
				ecs::EcsSystem::GetInstance().SetEntityManager(std::move(tmpEm));
			}
			if (ev.Id == 2)
			{
				glfwSetWindowShouldClose(WindowApp::GetInstance().GetWindow(), true);
			}
			em.RemoveEntity(em.GetEntity(ev));
		}
		for (auto l = em.GetComponents<ToggleEvent>(); !l.end(); ++l)
		{
			auto [ev] = *l;
			if (ev.Id == 3)
			{
				for (auto k = em.GetComponents<CreditsTableTag>(); !k.end(); ++k)
				{
					auto [tag] = *k;

					auto& tr = em.GetComponent<RectTransform>(tag);
					if (ev.state)
					{
						tr.pos = glm::vec2(0.7, 0.5);
					}
					else
					{
						tr.pos = glm::vec2(-5, -5);
					}
				}
			}
			em.RemoveEntity(em.GetEntity(ev));
		}
	}
}
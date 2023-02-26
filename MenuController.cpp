#include "Systems.h"
#include "Events.h"
#include <iostream>
#include "UiComponents.h"
#include "WindowApp.h"
#include "EntityLoader.h"

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
				SerializationSystem::LoadEntity(tmpEm, "Scenes\\Level1.txt");
				ecs::DefEcs().SetEntityManager(std::move(tmpEm));
			}
			if (ev.Id == 2)
			{
				glfwSetWindowShouldClose(WindowApp::GetInstance().GetWindow(), true);
			}
			if (ev.Id == 3)
			{
				for (auto k = ecs::DefEcs().entity.GetComponents<Tag>(); !k.end(); ++k)
				{
					auto [tag] = *k;
					if (tag.Name == "CreditsTable")
					{
						auto& tr = ecs::DefEcs().entity.GetComponent<RectTransform>(tag);
						tr.pos = glm::vec2(0.7, 0.5);
					}
				}
			}
			ecs::DefEcs().entity.RemoveEntity(ecs::DefEcs().entity.GetEntity(ev));
		}
	}
}
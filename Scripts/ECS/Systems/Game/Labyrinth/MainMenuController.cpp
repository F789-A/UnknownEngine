#include "ECS\Systems\Systems.h"
#include "ECS\Components\Game\Labyrinth\MainMenuTags.h"
#include "ECS\Components\UiComponents.h"
#include "Core\WindowApp.h"
#include "SerializationSystem\EntityLoader.h"

namespace Labyrinth
{
	void MainMenuController(ecs::EntityManager& em)
	{
		for (auto l = em.GetComponents<CompanyButtonTag, Button>(); !l.end(); ++l)
		{
			auto [t, button] = *l;
			if (button.clicked)
			{

			}
		}
		for (auto l = em.GetComponents<ArcadeButtonTag, Button>(); !l.end(); ++l)
		{
			auto [t, button] = *l;
			if (button.clicked)
			{
				ecs::EntityManager tmpEm;
				SerializationSystem::LoadEntity(tmpEm, "Scenes\\Labyrinth\\Labyrinth.txt");
				ecs::EcsSystem::GetInstance().SetEntityManager(std::move(tmpEm));
			}
		}
		for (auto l = em.GetComponents<TutorButtonTag, Button>(); !l.end(); ++l)
		{
			auto [t, button] = *l;
			if (button.clicked)
			{

			}
		}
		for (auto l = em.GetComponents<CreditsButtonTag, Button>(); !l.end(); ++l)
		{
			auto [t, button] = *l;
			if (button.clicked)
			{

			}
		}
		for (auto l = em.GetComponents<ExitButtonTag, Button>(); !l.end(); ++l)
		{
			auto [t, button] = *l;
			if (button.clicked)
			{
				glfwSetWindowShouldClose(WindowApp::GetInstance().GetWindow(), true);
			}
		}
	}
}
#include "Systems.h"
#include "ECS\Components\Skybox.h"
#include "ECS\Components\Camera.h"

void graphics::RenderSkyboxSystem(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<Skybox>(); !l.end(); ++l)
	{
		auto [skybox] = *l;
		for (auto k = em.GetComponents<MainCamera, Camera>(); !k.end(); ++k)
		{
			auto [m, camera] = *k;
			glm::mat4 view = camera.GetViewMatrix();
			glm::mat4 projection = camera.GetProjectionMatrix();
			view = glm::mat4(glm::mat3(view));
			skybox.RenderedSkybox.Draw(skybox.RenderedMaterial, projection * view);
		}
	}
}
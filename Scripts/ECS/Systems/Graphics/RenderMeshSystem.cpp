#include "ECS\Systems\Systems.h"

#include "ECS\Components\Transform.h"
#include "ECS\Components\RenderMesh.h"

void graphics::RenderMeshSystem(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<RenderMesh, Transform>(); !l.end(); ++l)
	{
		auto [render, transform] = *l;

		glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), transform.Scale);
		modelMatrix = glm::mat4_cast(transform.Rotation) * modelMatrix;
		modelMatrix = glm::translate(glm::mat4(1.0f), transform.Position) * modelMatrix;

		render.RenderedMesh.Draw(render.RenderMaterial, modelMatrix);
	}
}
#include "Systems.h"

#include "Transform.h"
#include "RenderMesh.h"

void RenderMeshSystem(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<RenderMesh, Transform>(); !l.end(); ++l)
	{
		auto [render, transform] = *l;

		glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), transform.Scale);
		//model = glm::mat4_cast(transform.Rotation) * model;
		modelMatrix = glm::translate(glm::mat4(1.0f), transform.Position) * modelMatrix;

		render.RenderedMesh.Draw(render.RenderMaterial, modelMatrix);
	}
}
#include "Systems.h"


#include "Transform.h"
#include "RenderMesh.h"

void RenderMeshSystem(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<RenderMesh, Transform>(); !l.end(); ++l)
	{
		auto [render, transform] = *l;
		render.RenderedMesh.Draw(render.RenderMaterial, transform.Position, transform.Rotation, transform.Scale);
	}
}
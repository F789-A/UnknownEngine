#include "Systems.h"
#include "RenderLine.h"

void ui::DrawLine(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<RenderLine>(); !l.end(); ++l)
	{
		auto [render] = *l;
		render.RenderedLine.Draw(render.RenderedMaterial, glm::mat4(1.0f));
	}
}
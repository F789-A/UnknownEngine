#include "LineDrawSystem.h"
#include "RenderLine.h"

void DrawLine()
{
	for (auto l = ECS::DefEcs_().entity.GetComponents<RenderLine>(); !l.end(); ++l)
	{
		auto [render] = *l;
		render.RenderedLine.Draw(render.RenderedMaterial);
	}
}
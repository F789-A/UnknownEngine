#pragma once
#include "GLMaterial.h"
#include "GLLineMesh.h"
#include "ecs_EntityManager.h"

struct RenderLine : ECS::Component<RenderLine>
{
	GLLineMesh RenderedLine;
	GLMaterial RenderedMaterial;
};
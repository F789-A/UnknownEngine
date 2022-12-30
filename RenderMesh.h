#pragma once

#include "ecs_EntityManager.h"

#include "GLMesh.h"
#include "GLMaterial.h"

struct RenderMesh : public ECS::Component<RenderMesh>
{
	GLMesh RenderedMesh;
	GLMaterial RenderMaterial;
};


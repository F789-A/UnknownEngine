#pragma once

#include "ILoopUpdate.h"
#include "Component.h"

#include "GLMesh.h"
#include "GLMaterial.h"
#include "Transform.h"
#include "EntityManager.h"

class RenderMesh: public Component<RenderMesh>, public ILoopUpdate<EGraphicLoop>
{
private:

public:
	GLMesh RenderedMesh;
	GLMaterial RenderMaterial;
	RenderMesh();

	RenderMesh(const RenderMesh&) = delete;
	RenderMesh operator=(const RenderMesh&) = delete;
	~RenderMesh() = default;

	RenderMesh(GLMesh renderedMesh, GLMaterial renderMaterial);

	void Update() override;
};
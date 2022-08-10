#pragma once

#include "ILoopUpdate.h"
#include "Component.h"
#include "Singleton.h"

#include "GLMesh.h"
#include "GLMaterial.h"
#include "Transform.h"
#include "EntityManager.h"

class RenderMesh: public ec::Component, public ILoopUpdate<UpdateType::GraphicLoop>
{
private:

public:
	GLMesh RenderedMesh;
	GLMaterial RenderMaterial;
	RenderMesh();

	RenderMesh(const RenderMesh&) = delete;
	RenderMesh operator=(const RenderMesh&) = delete;
	~RenderMesh() override = default;

	RenderMesh(GLMesh renderedMesh, GLMaterial renderMaterial);

	void Update() override;
};
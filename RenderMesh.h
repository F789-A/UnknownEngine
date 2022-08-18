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
	//std::weak_ptr<Transform> Transf;


public:
	GLMesh RenderedMesh;
	GLMaterial RenderMaterial;

	RenderMesh(GLMesh renderedMesh, GLMaterial renderMaterial);

	RenderMesh(const RenderMesh&) = delete;
	RenderMesh operator=(const RenderMesh&) = delete;

	~RenderMesh() override = default;



	void Update() override;
};
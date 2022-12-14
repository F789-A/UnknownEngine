#include "RenderMesh.h"

#include "SharedGraphicsResources.h"

RenderMesh::RenderMesh(GLMesh renderedMesh, GLMaterial renderMaterial):
	RenderedMesh(std::move(renderedMesh)),
	RenderMaterial(renderMaterial)
{}

void RenderMesh::Update()
{
	const auto& transf = GetLinkedEntity().GetComponent<Transform>();
	RenderedMesh.Draw(RenderMaterial, transf.Position, transf.Rotation, transf.Scale);
}
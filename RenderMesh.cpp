#include "RenderMesh.h"

RenderMesh::RenderMesh()
{
}

RenderMesh::RenderMesh(GLMesh renderedMesh, GLMaterial renderMaterial): RenderedMesh(std::move(renderedMesh)), RenderMaterial(renderMaterial)
{}

void RenderMesh::Update()
{
	Transform& transf = EntityManager::Manager().GetComponent<Transform>(LinkedEntity);
	RenderedMesh.Draw(*(RenderMaterial.Shader), transf.Position, transf.EulerAngle(), transf.Scale);
}
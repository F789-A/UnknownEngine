#include "RenderMesh.h"

#include "SharedGraphicsResources.h"

Singleton<SharedGraphicsResources> SinglRes()
{
	return Singleton<SharedGraphicsResources>();
}

RenderMesh::RenderMesh(): RenderMaterial(SinglRes()->GetShaderRef("Shaders\\DefShader.ueshad"))
{}


RenderMesh::RenderMesh(GLMesh renderedMesh, GLMaterial renderMaterial): RenderedMesh(std::move(renderedMesh)), RenderMaterial(renderMaterial)
{}

void RenderMesh::Update()
{
	Transform& transf = GetLinkedEntity().GetComponent<Transform>();
	RenderedMesh.Draw(*(RenderMaterial.Shader), transf.Position, transf.EulerAngle(), transf.Scale);
}
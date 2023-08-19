#pragma once
#include "EcsSystem.h"
#include "GLMesh.h"
#include "GLMaterial.h"
#include "SharedGraphicsResources.h"

struct Skybox : public ecs::Component<Skybox>
{
	GLMesh RenderedSkybox;
	GLMaterial RenderedMaterial;

    static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
    {
        auto& sk = em.GetComponent<Skybox>(a);

        static std::vector<Vertex> SkyboxVertices = { // replace to model
		Vertex({-1.0f,  1.0f, -1.0f}),
		Vertex({-1.0f, -1.0f, -1.0f}),
		Vertex({1.0f, -1.0f, -1.0f}),
		Vertex({1.0f, -1.0f, -1.0f}),
		Vertex({1.0f,  1.0f, -1.0f}),
		Vertex({-1.0f,  1.0f, -1.0f}),

		Vertex({-1.0f, -1.0f,  1.0f}),
		Vertex({-1.0f, -1.0f, -1.0f}),
		Vertex({-1.0f,  1.0f, -1.0f}),
		Vertex({-1.0f,  1.0f, -1.0f}),
		Vertex({-1.0f,  1.0f,  1.0f}),
		Vertex({-1.0f, -1.0f,  1.0f}),

		Vertex({ 1.0f, -1.0f, -1.0f}),
		Vertex({ 1.0f, -1.0f,  1.0f}),
		Vertex({ 1.0f,  1.0f,  1.0f}),
		Vertex({ 1.0f,  1.0f,  1.0f}),
		Vertex({ 1.0f,  1.0f, -1.0f}),
		Vertex({ 1.0f, -1.0f, -1.0f}),

		Vertex({-1.0f, -1.0f,  1.0f}),
		Vertex({-1.0f,  1.0f,  1.0f}),
		Vertex({ 1.0f,  1.0f,  1.0f}),
		Vertex({ 1.0f,  1.0f,  1.0f}),
		Vertex({ 1.0f, -1.0f,  1.0f}),
		Vertex({-1.0f, -1.0f,  1.0f}),

		Vertex({-1.0f,  1.0f, -1.0f}),
		Vertex({ 1.0f,  1.0f, -1.0f}),
		Vertex({ 1.0f,  1.0f,  1.0f}),
		Vertex({ 1.0f,  1.0f,  1.0f}),
		Vertex({-1.0f,  1.0f,  1.0f}),
		Vertex({-1.0f,  1.0f, -1.0f}),

		Vertex({-1.0f, -1.0f, -1.0f}),
		Vertex({-1.0f, -1.0f,  1.0f}),
		Vertex({ 1.0f, -1.0f, -1.0f}),
		Vertex({ 1.0f, -1.0f, -1.0f}),
		Vertex({-1.0f, -1.0f,  1.0f}),
		Vertex({ 1.0f, -1.0f,  1.0f})
		};

		std::vector<unsigned int> vertices;
		for (int i = 0; i < 36; i++)
		{
			vertices.push_back(i);
		}

        Singleton<SharedGraphicsResources> singlRes;

        sk.RenderedSkybox = GLMesh(SkyboxVertices, vertices);
        sk.RenderedMaterial = GLMaterial(singlRes->GetMaterial(res["RenderMaterial"]));
    }
};
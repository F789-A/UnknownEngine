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

        static std::vector<Vertex> SkyboxVertices = { //TODO: replace to model
			Vertex({-1.0f,  1.0f, -1.0f}),
			Vertex({-1.0f, -1.0f, -1.0f}),
			Vertex({ 1.0f, -1.0f, -1.0f}),
			Vertex({ 1.0f,  1.0f, -1.0f}),
			Vertex({-1.0f, -1.0f,  1.0f}),
			Vertex({-1.0f,  1.0f,  1.0f}),
			Vertex({ 1.0f, -1.0f,  1.0f}),
			Vertex({ 1.0f,  1.0f,  1.0f}),
		};

		static const std::vector<unsigned int> vertices = {0, 1, 2, 2, 3, 0, 4, 1, 0, 0, 5, 4, 2, 6, 7, 7, 3, 2, 4, 5, 7, 7, 6, 4, 0, 3, 7, 7
		, 5, 0, 1, 4, 2, 2, 4, 6};

        Singleton<SharedGraphicsResources> singlRes;

        sk.RenderedSkybox = GLMesh(SkyboxVertices, vertices);
        sk.RenderedMaterial = GLMaterial(singlRes->GetMaterial(res["RenderMaterial"]));
    }
};
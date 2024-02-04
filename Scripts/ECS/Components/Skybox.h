#pragma once
#include "ECS\EcsSystem.h"
#include "Graphics\GLMesh.h"
#include "Graphics\GLMaterial.h"
#include "SerializationSystem\SharedGraphicsResources.h"

struct Skybox : public ecs::Component<Skybox>
{
	static constexpr std::string_view ComponentName = "Skybox";

	GLMesh RenderedSkybox;
	GLMaterial RenderedMaterial;

    static void Load(ecs::EntityManager& em, int a, std::map<std::string, std::string>& res)
    {
        auto& sk = em.GetComponent<Skybox>(a);

        static const std::vector<Vertex> SkyboxVertices = { //TODO: replace to model
			Vertex({-1.0f,  1.0f, -1.0f}),
			Vertex({-1.0f, -1.0f, -1.0f}), //   0------3
			Vertex({ 1.0f, -1.0f, -1.0f}), //  /|	  /|
			Vertex({ 1.0f,  1.0f, -1.0f}), // 5------7 |
			Vertex({-1.0f, -1.0f,  1.0f}), // |	| 	 | |
			Vertex({-1.0f,  1.0f,  1.0f}), // |	1----|-2
			Vertex({ 1.0f, -1.0f,  1.0f}), // |/	 |/
			Vertex({ 1.0f,  1.0f,  1.0f}), // 4------6
		};

		static const std::vector<unsigned int> vertices = { 0, 1, 2, 2, 3, 0, 4, 1, 0, 0, 5, 4, 2, 6, 7, 7, 3, 2, 4, 5, 7, 7, 6, 4, 
			0, 3, 7, 7, 5, 0, 1, 4, 2, 2, 4, 6 };

        Singleton<SharedGraphicsResources> singlRes;

        sk.RenderedSkybox = GLMesh(SkyboxVertices, vertices);
        sk.RenderedMaterial = GLMaterial(singlRes->GetMaterial(res["RenderMaterial"]));
    }
};
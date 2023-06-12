#include "GraphicCore.h"

#include "Camera.h"
#include "Mesh.h"

GraphicCore::GraphicCore(GLFWwindow* window) : Window(window)
{
	Height = WindowApp::GetInstance().Height();
	Width = WindowApp::GetInstance().Width();
	ecsS = &ecs::DefEcs();
	//init advice
	Singleton<SharedGraphicsResources> SinglRes;
	
	GLShader& shad = SinglRes->GetShaderRef("Shaders\\BlendUiAndScene.ueshad");
	BlendSceneMaterial = GLMaterial(shad);
	BlendSceneMaterial.Textures.insert({"sceneTexture", &sceneFramebuffer.texture });
	BlendSceneMaterial.Textures.insert({"uiTexture", &uiFramebuffer.texture });

	GLCubemapTexture& skMap = SinglRes->GetGLCubemapRef("skybox/DefaultSkybox.ueskybox");
	GLShader& skShad = SinglRes->GetShaderRef("Shaders/Skybox.ueshad");
	CurrentSkybox = std::make_unique<Skybox>(skMap, skShad);

	PostProcesses.push_back(SinglRes->GetMaterial("Materials/Pixelization.uemat"));
	for (auto& l : PostProcesses)
	{
		l.Textures.insert({"screenTexture", &sceneFramebuffer.texture});
	}

	//init GL
	glClearColor(0.2f, 0.3f, 0.3f, 0.0f);
	//
	glGenBuffers(1, &uniformCameraBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformCameraBlock);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uniformCameraBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	//
	int aspect[2] {Width, Height};
	glGenBuffers(1, &Uniform_Shaders_Parameters);
	glBindBuffer(GL_UNIFORM_BUFFER, Uniform_Shaders_Parameters);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, Uniform_Shaders_Parameters);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 2*sizeof(GLuint), aspect);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GraphicCore& GraphicCore::GetInstance()
{
	static GraphicCore  instance(WindowApp::GetInstance().GetWindow());
	return instance;
}

void GraphicCore::UpdateGraphic()
{
	static std::vector<Vertex2D> vertices = {
		{{-1, 1, 0}, {0, 1}},
		{{-1, -1, 0}, {0, 0}},
		{{ 1, -1, 0}, {1, 0}},
		{{ 1, 1, 0}, {1, 1}}
	};

	static std::vector<GLuint> ind = {0, 1, 2, 0, 2, 3};
	static GLMesh screenPlane(vertices, ind);

	Camera* cam = nullptr;
	for (auto l = ecs::DefEcs().entity.GetComponents<MainCamera, Camera>(); !l.end(); ++l)
	{
		auto [m, camera] = *l;
		cam = &camera;
	}
	glm::mat4 view = cam->GetViewMatrix();
	glm::mat4 projection = glm::perspective(cam->FOV, (float)Width / Height, 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformCameraBlock);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Scene pass
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFramebuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto& func : mainPass)
	{
		func(ecsS->entity);
	}
	if (CurrentSkybox != nullptr)
	{
		CurrentSkybox->Draw(view, projection);
	}

	// Post process pass
	if (EnablePostProcessing)
	{
		BlendSceneMaterial.Textures["sceneTexture"] = &postProcessFramebuffer.texture;
		glBindFramebuffer(GL_FRAMEBUFFER, postProcessFramebuffer.fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		screenPlane.Draw(PostProcesses[0], glm::mat4(1.0f));
	}
	else
	{
		BlendSceneMaterial.Textures["sceneTexture"] = &sceneFramebuffer.texture;
	}

	// UI pass
	glBindFramebuffer(GL_FRAMEBUFFER, uiFramebuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto& uiFunc : UiPass)
	{
		uiFunc(ecsS->entity);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Draw all on screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screenPlane.Draw(BlendSceneMaterial, glm::mat4(1.0f));

	glfwSwapBuffers(Window);
}
#include "GraphicCore.h"

#include "Core\WindowApp.h"
#include "SerializationSystem\SharedGraphicsResources.h"
#include "Assets\Mesh.h"

GraphicCore::GraphicCore(GLFWwindow* window) : Window(window)
{
	Height = WindowApp::GetInstance().Height();
	Width = WindowApp::GetInstance().Width();
	Aspect = (double)Height / Width;
	//init advice
	Singleton<SharedGraphicsResources> SinglRes;
	GLShader& shad = SinglRes->GetShaderRef("Shaders\\BlendUiAndScene.ueshad");
	BlendSceneMaterial = GLMaterial(shad);
	BlendSceneMaterial.Textures.insert({"sceneTexture", &sceneFramebuffer.texture });
	BlendSceneMaterial.Textures.insert({"uiTexture", &uiFramebuffer.texture });

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
		{{-1,  1, 0}, {0, 1}},
		{{-1, -1, 0}, {0, 0}},
		{{ 1, -1, 0}, {1, 0}},
		{{ 1,  1, 0}, {1, 1}}
	};

	static std::vector<GLuint> ind = {0, 1, 2, 0, 2, 3};
	static GLMesh screenPlane(vertices, ind);

	auto matrices = GetCameraMatrices();
	glBindBuffer(GL_UNIFORM_BUFFER, uniformCameraBlock);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(matrices.first));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(matrices.second));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Scene pass
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFramebuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mainPassFunc();

	// Post process pass
	const auto& postProcesses = GetPostProcesses();
	if (postProcesses.size() != 0)
	{
		BlendSceneMaterial.Textures["sceneTexture"] = &postProcessFramebuffer.texture;
		glBindFramebuffer(GL_FRAMEBUFFER, postProcessFramebuffer.fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		BlendSceneMaterial.Textures["sceneTexture"] = &sceneFramebuffer.texture;
	}
	for (auto postProcess : postProcesses)
	{
		postProcess->Textures["screenTexture"] = &sceneFramebuffer.texture;
		screenPlane.Draw(*postProcess, glm::mat4(1.0f));
	}
	// UI pass
	glBindFramebuffer(GL_FRAMEBUFFER, uiFramebuffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	uiPassFunc();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Draw all on screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screenPlane.Draw(BlendSceneMaterial, glm::mat4(1.0f));

	glfwSwapBuffers(Window);
}
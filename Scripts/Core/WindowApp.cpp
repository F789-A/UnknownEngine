#include "WindowApp.h"
#include "Logger.h"
#include <iostream>
#include <cassert>
//Init
WindowApp::WindowApp(unsigned int width, unsigned int height, std::string name)
{
	SetupWindowApp(width, height, name);
}

inline void WindowApp::SetupWindowApp(unsigned int width, unsigned int height, std::string name)
{
	Singleton<Logger> log;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GlVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GlVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	if (Window == nullptr)
	{
		log->Log("Failed to create GLFW window");
		assert(false);
	}

	glfwMakeContextCurrent(Window);

	bool loadedGLL = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	if (!loadedGLL)
	{
		log->Log("Failed to initialize GLAD");
		assert(false);
	}

	glfwGetFramebufferSize(Window, &WindowWidth, &WindowHeight);
	glViewport(0, 0, WindowWidth, WindowHeight);
	//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

WindowApp& WindowApp::GetInstance()
{
	static WindowApp instance;
	return instance;
}

void WindowApp::CreateWindow()
{}

//Getters
int WindowApp::Width()
{
	return WindowWidth;
}
int WindowApp::Height()
{
	return WindowHeight;
}
GLFWwindow* WindowApp::GetWindow()
{
	return Window;
}

bool WindowApp::ShouldClose()
{
	return glfwWindowShouldClose(Window);
}

void WindowApp::TerminateApp()
{
	glfwTerminate();
}
#include "WindowApp.h"

//Init
WindowApp::WindowApp(unsigned int width, unsigned int height, std::string name)
{
	SetupWindowApp(width, height, name);
}

inline void WindowApp::SetupWindowApp(unsigned int width, unsigned int height, std::string name)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GlVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GlVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	GlAssert(Window != nullptr, "Failed to create GLFW window");

	glfwMakeContextCurrent(Window);

	bool loadedGLL = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	GlAssert(loadedGLL, "Failed to initialize GLAD");

	glfwGetFramebufferSize(Window, &WindowWidth, &WindowHeight);
	glViewport(0, 0, WindowWidth, WindowHeight);

	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

WindowApp& WindowApp::GetInstance()
{
	static WindowApp instance;
	return instance;
}

void WindowApp::CreateWindow()
{}

//Getters
unsigned int WindowApp::Width()
{
	return WindowWidth;
}
unsigned int WindowApp::Height()
{
	return WindowHeight;
}
GLFWwindow* WindowApp::GetWindow()
{
	return Window;
}
float WindowApp::GetTime()
{
	return glfwGetTime();
}
bool WindowApp::ShouldClose()
{
	return glfwWindowShouldClose(Window);
}

void WindowApp::TerminateApp()
{
	glfwTerminate();
}
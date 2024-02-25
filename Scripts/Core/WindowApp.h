#pragma once
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <string>

class WindowApp
{
private:
	GLFWwindow* Window;

	const unsigned int GlVersionMajor = 4;
	const unsigned int GlVersionMinor = 4;

	GLint WindowHeight;
	GLint WindowWidth;

	WindowApp(unsigned int width = 1000, unsigned int height = 750, std::string name = "Window");

	WindowApp operator=(const WindowApp&) = delete;
	WindowApp(const WindowApp&) = delete;
	WindowApp(WindowApp&&) = delete;
	WindowApp& operator=(WindowApp&&) = delete;

	~WindowApp() = default;

	void SetupWindowApp(unsigned int width, unsigned int height, std::string name);

public:
	static WindowApp& GetInstance();

	void CreateWindow();

	int Width();
	int Height();

	GLFWwindow* GetWindow();
	bool ShouldClose();
	void TerminateApp();
};
#pragma once

#include <iostream>
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <cassert>

class WindowApp
{
private:
	GLFWwindow* Window;

	const unsigned int GlVersionMajor = 4;
	const unsigned int GlVersionMinor = 4;

	GLint WindowHeight;
	GLint WindowWidth;

	WindowApp(unsigned int width = 800, unsigned int height = 600, std::string name = "Window");
	WindowApp operator=(const WindowApp&) = delete;
	WindowApp(const WindowApp&) = delete;
	WindowApp(WindowApp&&) = delete;
	WindowApp& operator=(WindowApp&&) = delete;

	~WindowApp() = default;

	void SetupWindowApp(unsigned int width, unsigned int height, std::string name);

public:
	static WindowApp& GetInstance();

	void CreateWindow();

	unsigned int Width();
	unsigned int Height();

	GLFWwindow* GetWindow();
	float GetTime();
	bool ShouldClose();
	void TerminateApp();
};
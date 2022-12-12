#include "Input.h"

void Input::ProcessMousePos()
{
	glfwGetCursorPos(WindowApp::GetInstance().GetWindow(), &MousePosX, &MousePosY);
}

Input::Input(): NameToKey()
{
	//glfwSetInputMode(WindowApp::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	Input::ProcessMousePos();
}

Input& Input::GetInstance()
{
	static Input  instance;
	return instance;
}

void Input::UpdateInput()
{
	PrevMousePosX = MousePosX;
	PrevMousePosY = MousePosY;
	ProcessMousePos();
	glfwPollEvents(); // порядок? не только ввод?
}


//ForUsers
void Input::BindKey(std::string name, int key)
{
	NameToKey[name] = key;
}

bool Input::GetButton(std::string name, int mode) const
{
	return GetKey(NameToKey.at(name) , mode);
}

bool Input::GetKey(int key, int mode) const
{
	return glfwGetKey(WindowApp::GetInstance().GetWindow(), key) == mode;
}

bool Input::GetKey(int key, PressMode mode) const
{
	return glfwGetKey(WindowApp::GetInstance().GetWindow(), key) == (int)mode;
}

void Input::EnableCursor()
{
	glfwSetInputMode(WindowApp::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Input::DisableCursor()
{
	glfwSetInputMode(WindowApp::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

//Getters
float Input::MouseDeltaX() const
{
	return MousePosX - PrevMousePosX;
}

float Input::MouseDeltaY() const
{
	return MousePosY - PrevMousePosY;
}

float Input::GetMousePosX() const
{
	return MousePosX;
}

float Input::GetMousePosY() const
{
	return MousePosY;
}
#include "Input.h"

Input::Input() : NameToKey()
{
	glfwSetInputMode(WindowApp::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

void Input::ProcessMousePos()
{
	glfwGetCursorPos(WindowApp::GetInstance().GetWindow(), &MousePosX, &MousePosY);
}

void Input::BindKey(std::string name, int key)
{
	NameToKey[name] = key;
}

bool Input::GetButton(std::string name, int mode)
{
	return GetKey(NameToKey[name], mode);
}

bool Input::GetKey(int key, int mode)
{
	return glfwGetKey(WindowApp::GetInstance().GetWindow(), key) == mode;
}

float Input::MouseDeltaX()
{
	return MousePosX - PrevMousePosX;
}

float Input::MouseDeltaY()
{
	return MousePosY - PrevMousePosY;
}

float Input::GetMousePosX()
{
	return MousePosX;
}

float Input::GetMousePosY()
{
	return MousePosY;
}
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
	glfwPollEvents(); // ???????? ?? ?????? ?????
}

void Input::ProcessMousePos()
{
	glfwGetCursorPos(WindowApp::GetInstance().GetWindow(), &MousePosX, &MousePosY);
}

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
#include "Input.h"
#include "WindowApp.h"

Input& Input::GetInstance()
{
	static Input  instance(WindowApp::GetInstance().GetWindow());
	return instance;
}

Input::Input(GLFWwindow* window): window(window)
{
	Input::ProcessMousePos();
}

void Input::UpdateInput()
{
	for (auto l : KeyState)
	{
		UpdateKey(l.first);
	}
	ProcessMousePos();
	glfwPollEvents();
}

void Input::ProcessMousePos()
{
	PrevMousePosX = MousePosX;
	PrevMousePosY = MousePosY;
	glfwGetCursorPos(window, &MousePosX, &MousePosY);
}

void Input::UpdateKey(int key)
{
	bool rawPress;
	if (GLFW_MOUSE_BUTTON_1 <= key && key <= GLFW_MOUSE_BUTTON_8)
	{
		rawPress = static_cast<bool>(glfwGetMouseButton(window, key));
	}
	else
	{
		rawPress = static_cast<bool>(glfwGetKey(window, key));
	}

	static const std::map<std::pair<PressMode, bool>, PressMode> transits = {
		{{PressMode::NotPress, true}, PressMode::Press},
		{{PressMode::Press, true}, PressMode::Repeat},
		{{PressMode::Repeat, true}, PressMode::Repeat},
		{{PressMode::Release, true}, PressMode::Press},
		{{PressMode::NotPress, false}, PressMode::NotPress},
		{{PressMode::Press, false}, PressMode::NotPress},
		{{PressMode::Repeat, false}, PressMode::Release},
		{{PressMode::Release, false}, PressMode::NotPress}
	};

	KeyState[key] = transits.at({KeyState[key], rawPress});
}

//ForUsers
void Input::BindKey(std::string name, int key)
{
	NameToKey[name] = key;
}

bool Input::GetButton(std::string name, PressMode mode)
{
	return GetKey(NameToKey.at(name), mode);
}

bool Input::GetKey(int key, PressMode mode)
{
	if (KeyState.find(key) == KeyState.end())
	{
		UpdateKey(key);
	}
	return KeyState.at(key) == mode;
}

void Input::SetCursorMode(CursorMode cursorMode)
{
	glfwSetInputMode(window, GLFW_CURSOR, static_cast<int>(cursorMode));
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
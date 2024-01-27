#pragma once
#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include <map>
#include <string>

class Input
{
public:
	enum class PressMode
	{
		NotPress,
		Press,
		Repeat,
		Release
	};

	enum class CursorMode
	{
		Normal = GLFW_CURSOR_NORMAL,
		Hidden = GLFW_CURSOR_HIDDEN,
		Disable = GLFW_CURSOR_DISABLED
	};

private:
	GLFWwindow* window;

	std::map<int, PressMode> KeyState;
	std::map<std::string, int> NameToKey;

	Input(GLFWwindow*);

	Input operator=(const Input&) = delete;
	Input(const Input&) = delete;
	~Input() = default;

	void ProcessMousePos();
	void UpdateKey(int key);

	GLdouble PrevMousePosX;
	GLdouble PrevMousePosY;
	GLdouble MousePosX;
	GLdouble MousePosY;
	
public:
	float MouseDeltaX() const;
	float MouseDeltaY() const;

	float GetMousePosX() const;
	float GetMousePosY() const ;

	void BindKey(std::string name, int key);
	bool GetButton(std::string name, PressMode mode);
	bool GetKey(int key, PressMode mode);

	void SetCursorMode(CursorMode cursorMode);

	static Input& GetInstance();

	void UpdateInput();
};
#pragma once
#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include "WindowApp.h"
#include <map>
#include <string>

class Input
{
private:
	std::map<std::string, int> NameToKey;

	Input();
	Input operator=(const Input&) = delete;
	Input(const Input&) = delete;
	~Input() = default;

	void ProcessMousePos();

	GLdouble PrevMousePosX;
	GLdouble PrevMousePosY;
	GLdouble MousePosX;
	GLdouble MousePosY;
	
public:
	enum class PressMode
	{
		Press = GLFW_PRESS,
		Repeat = GLFW_REPEAT,
		Release = GLFW_RELEASE
	};

	float MouseDeltaX() const;
	float MouseDeltaY() const;

	float GetMousePosX() const;
	float GetMousePosY()const ;

	void BindKey(std::string name, int key);
	bool GetButton(std::string name, int mode) const;
	bool GetKey(int key, int mode) const;
	bool GetKey(int key, PressMode mode) const;
	void EnableCursor();
	void DisableCursor();

	static Input& GetInstance();

	void UpdateInput();
};
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
	float MouseDeltaX();
	float MouseDeltaY();

	float GetMousePosX();
	float GetMousePosY();

	void BindKey(std::string name, int key);
	bool GetButton(std::string name, int mode);
	bool GetKey(int key, int mode);

	static Input& GetInstance();

	void UpdateInput();
};
#pragma once
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "AppTime.h"
#include "GLLineMesh.h"

void PerspectiveBuild()
{
	static glm::vec2 center(0, 0);
	static float height = 0.3f;
	float sens = 1.0 * AppTime::GetDeltaTime();
	if (Input::GetInstance().GetKey(GLFW_KEY_Y, Input::PressMode::Press))
	{
		height += sens;
	}
	if (Input::GetInstance().GetKey(GLFW_KEY_H, Input::PressMode::Press))
	{
		height -= sens;
	}
	if (Input::GetInstance().GetKey(GLFW_KEY_RIGHT, Input::PressMode::Press))
	{
		center.x += sens;
	}
	if (Input::GetInstance().GetKey(GLFW_KEY_LEFT, Input::PressMode::Press))
	{
		center.x -= sens;
	}
	if (Input::GetInstance().GetKey(GLFW_KEY_UP, Input::PressMode::Press))
	{
		center.y += sens;
	}
	if (Input::GetInstance().GetKey(GLFW_KEY_DOWN, Input::PressMode::Press))
	{
		center.y -= sens;
	}
	height = height > 0 ? height : 0;
	std::vector<glm::vec2> corner = { {-1, 1}, {1, 1}, {1, -1}, {-1, -1} };
	float cosA = (center.x - corner[0].x) / (glm::length(center - corner[0]) * 1);
	float ctgA = cosA / std::sqrt(1 - cosA * cosA);
	float cosB = (-center.x + corner[1].x) / (glm::length(center - corner[1]) * 1);
	float ctgB = cosB / std::sqrt(1 - cosB * cosB);
	float cosC = (-center.x + corner[2].x) / (glm::length(center - corner[2]) * 1);
	float tgC = std::sqrt(1 - cosC * cosC)/cosC;
	std::vector<glm::vec2> quad(4);
	quad[0] = glm::vec2(-1 + height * ctgA, 1-height);
	quad[1] = glm::vec2(1 - height * ctgB, 1-height);
	quad[2] = glm::vec2(1 - height * ctgB, -1 + height * ctgB*tgC);
	quad[3] = glm::vec2(-1 + height * ctgA, -1 + height * ctgB * tgC);
	std::vector<glm::vec2> arr =
	{
		//
		quad[0], quad[1],
		quad[1], quad[2],
		quad[2], quad[3],
		quad[3], quad[0],
		//
		corner[0], quad[0],
		corner[1], quad[1],
		corner[2], quad[2],
		corner[3], quad[3]
	};
	static GLLineMesh a(arr, {});
	Singleton<SharedGraphicsResources> singlRes;
	singlRes->GetMaterial("Materials\\fuck.uemat").Use();
	//a.Draw(singlRes->GetMaterial("Materials\\fuck.uemat"), arr);
}


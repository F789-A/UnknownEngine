#pragma once
#include "GLMaterial.h"
#include <glm/glm.hpp>

void DrawUI(const GLMaterial& material, const glm::vec2& position, const glm::vec2& scale); // костыль

namespace ui
{
	void DrawImage();
	void ProcessButtons();
	void DrawUIImage();
}

void RenderMeshSystem();

void EscapeHandler();

void DrawLine();

void CameraControllerSystem();
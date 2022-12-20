#pragma once
#include "RectTransformComponent.h"
#include "GlMaterial.h"
#include "GLMesh.h"
#include "SharedGraphicsResources.h"
#include "Input.h"

void DrawUI(const GLMaterial& material, const glm::vec2& position, const glm::vec2& scale);

namespace ui
{
	void DrawImage();
	void ProcessButtons();
	void DrawUIImage();
}




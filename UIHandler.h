#pragma once
#include "RectTransformComponent.h"
#include "GlMaterial.h"
#include "GLMesh.h"
#include "SharedGraphicsResources.h"
#include "Input.h"

void DrawUI(const GLMaterial& material, const glm::vec2& position, const glm::vec2& scale);

class ImageDrawler : public ECS::SystemBase
{
public:
	static int priorityId;
	void Update() override
	{
		for (auto l = ECS::DefEcs_().entity.GetComponents<Image, RectTransform>(); !l.end; ++l)
		{
			auto [image, transf] = *l;

			Singleton<SharedGraphicsResources> singlRes;
			std::string a = "Materials\\DefaultUI.uemat";
			DrawUI(singlRes->GetMaterial(a), transf.pos, transf.size);
		}
	}
};

class ButtonProcessor : public ECS::SystemBase
{
public:
	static int priorityId;
	void Update() override
	{
		for (auto l = ECS::DefEcs_().entity.GetComponents<Button, RectTransform>(); !l.end; ++l)
		{
			auto [button, transf] = *l;
			float x = Input::GetInstance().GetMousePosX() /800;
			float y = Input::GetInstance().GetMousePosY() / 600;  
			glfwSetInputMode(WindowApp::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			if (Input::GetInstance().GetKey(GLFW_KEY_Z, Input::PressMode::Press))
			{
				if (transf.pos.x - transf.size.x * 0.25 <= x && x <= transf.pos.x + transf.size.x * 0.25
					&& transf.pos.y - transf.size.y * 0.25 <= y && y <= transf.pos.y + transf.size.y * 0.25)
				{
					std::cout << button.identifier << std::endl;
				}
			}
		}
	}
};

namespace ui_engine
{
	void DrawUIImage();
}




#include "Systems.h"

#include "UiComponents.h"
#include "GlMaterial.h"
#include "GLMesh.h"
#include "SharedGraphicsResources.h"
#include "Input.h"

void ui::DrawImage()
{
	for (auto l = ECS::DefEcs_().entity.GetComponents<Image, RectTransform>(); !l.end(); ++l)
	{
		auto [image, transf] = *l;

		Singleton<SharedGraphicsResources> singlRes;
		std::string a = "Materials\\DefaultUI.uemat";
		DrawUI(singlRes->GetMaterial(a), transf.pos, transf.size);
	}
}

void ui::ProcessButtons()
{
	for (auto l = ECS::DefEcs_().entity.GetComponents<Button, RectTransform>(); !l.end(); ++l)
	{
		auto [button, transf] = *l;
		float x = Input::GetInstance().GetMousePosX() / 800;
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

void ui::DrawUIImage()
{
	for (auto l = ECS::DefEcs_().entity.GetComponents<Image, RectTransform>(); !l.end(); ++l)
	{
		auto [image, transf] = *l;

		Singleton<SharedGraphicsResources> singlRes;
		std::string a = "Materials\\DefaultUI.uemat";
		DrawUI(singlRes->GetMaterial(a), transf.pos, transf.size);
	}
}

void DrawUI(const GLMaterial& material, const glm::vec2& position, const glm::vec2& scale)
{
	Singleton<SharedGraphicsResources> singlRes;

	material.Use();

	glm::mat4 model = glm::mat4(1.0f);
	
	model = glm::translate(model, glm::vec3(position.x*2-1, (1-position.y)*2-1, 0));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1));

	glUniformMatrix4fv(glGetUniformLocation(material.Shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	static GLMesh uiplane(singlRes->ModelCont.GetModelRef("Models\\Plane.obj").Meshes[0]);

	glBindVertexArray(uiplane.VAO);
	glDrawElements(GL_TRIANGLES, uiplane.IndicesSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
#include "UIHandler.h"


int ButtonProcessor::priorityId = -1;
int ImageDrawler::priorityId = -1;

void ui_engine::DrawUIImage()
{
	for (auto l = ECS::DefEcs_().entity.GetComponents<Image, RectTransform>(); !l.end; ++l)
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
#include "ECS\Systems\Systems.h"

#include <glm/gtc/matrix_transform.hpp>

#include "ECS\Components\UiComponents.h"
#include "ECS\Components\Events.h"

#include "Graphics\GlMaterial.h"
#include "Graphics\GLMesh.h"
#include "Graphics\GraphicCore.h"
#include "SerializationSystem\SharedGraphicsResources.h"
#include "Core\Input.h"

glm::vec3 ScrToNormalize(glm::vec2 pos)
{
	return glm::vec3(pos.x * 2 - 1, pos.y * 2 - 1, 0);
}

void DrawUI(const GLMaterial& material, const glm::vec2& position, const glm::vec2& scale, float priority)
{
	static GLMesh uiplane(
		{
		Vertex2D({-1, 1, 0}, {0, 1}),
		Vertex2D({-1, -1, 0}, {0, 0}),
		Vertex2D({1, -1, 0}, {1, 0}),
		Vertex2D({1, 1, 0}, {1, 1})
		},
		{ 0, 1, 2, 0, 2, 3 });

	glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1));
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position.x * 2 - 1, position.y * 2 - 1, 1 - (float)priority / 100.0f)) * modelMatrix;

	uiplane.Draw(material, modelMatrix);
}

void DrawUI(const GLMaterial& material, std::vector<Vertex2D> vertices, int priority)
{
	static GLMesh uiplane(vertices, { 0, 1, 2, 0, 2, 3 });
	std::vector<GLfloat> verts;
	for (auto l : vertices)
	{
		verts.push_back(l.position.x);
		verts.push_back(l.position.y);
		verts.push_back(l.position.z);
		verts.push_back(l.texCoords.x);
		verts.push_back(l.texCoords.y);
	}
	uiplane.SetVertexData(verts);

	glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	//model = glm::mat4_cast(glm::quat(1, 0, 0, 0)) * model;
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f - (float)priority / 100.0f)) * modelMatrix;

	uiplane.Draw(material, modelMatrix);
}

std::vector<glm::vec2> PixToScreenCoord(glm::ivec2 st, glm::ivec2 size)
{
	std::vector<glm::vec2> result(4);
	result[0] = glm::vec2((float)st.x / 512, (float)st.y / 512);
	result[1] = glm::vec2((float)st.x / 512, (float)(st.y + size.y) / 512);
	result[2] = glm::vec2((float)(st.x + size.x) / 512, (float)(st.y + size.y) / 512);
	result[3] = glm::vec2((float)(st.x + size.x) / 512, (float)(st.y) / 512);
	return result;
}

void ui::ProcessButtons(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<Button, RectTransform>(); !l.end(); ++l)
	{
		auto [button, transf] = *l;
		float x = Input::GetInstance().GetMousePosX() / GraphicCore::GetInstance().Width;
		float y = 1-Input::GetInstance().GetMousePosY() / GraphicCore::GetInstance().Height;
		Input::GetInstance().SetCursorMode(Input::CursorMode::Normal);

		button.clicked = false;

		if (Input::GetInstance().GetButton("MouseLeft", Input::PressMode::Release ))
		{
			if (transf.pos.x - transf.size.x * 0.5 <= x && x <= transf.pos.x + transf.size.x * 0.5
				&& transf.pos.y - transf.size.y * 0.5 <= y && y <= transf.pos.y + transf.size.y * 0.5)
			{
				if (em.CheckComponents<Toggle>(em.GetEntity(button)))
				{
					auto& toggle = em.GetComponent<Toggle>(em.GetEntity(button));
					toggle.active = !toggle.active;

					int ent = em.AddEntity<ToggleEvent>();
					em.GetComponent<ToggleEvent>(ent).Id = button.identifier;
					em.GetComponent<ToggleEvent>(ent).state = toggle.active;
				}
				else
				{
					int ent = em.AddEntity<Event>();
					em.GetComponent<Event>(ent).Id = button.identifier;
				}

				button.clicked = true;
			}
		}
	}
}

void ui::DrawUIImage(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<Image, RectTransform>(); !l.end(); ++l)
	{
		auto [image, transf] = *l;
		transf.UpdateMatrix();
		DrawUI(image.Material, transf.globalPos, transf.size, transf.priority);
	}
}

void ui::DrawText(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<Text, RectTransform>(); !l.end(); ++l)
	{
		auto [text, transf] = *l;
		transf.UpdateMatrix();

		float halfWidth = (float)GraphicCore::GetInstance().Width / 2.0f;
		float halfHeight = (float)GraphicCore::GetInstance().Height / 2.0f;

		float lineSpace = ((float)text.font->FontSize *2) / GraphicCore::GetInstance().Height;

		glm::vec2 curPos = ScrToNormalize(glm::vec2(transf.globalPos.x - transf.size.x / 2, transf.globalPos.y + transf.size.y / 2)); // ������� ����� ����

		curPos += glm::vec2(0, -lineSpace);

		for (auto k : text.text)
		{
			Character ch = text.font->Characters[k];

			if (curPos.x + (float)ch.Advance / halfWidth > (transf.globalPos.x + transf.size.x / 2) * 2 - 1)
			{
				curPos.y -= lineSpace;
				curPos.x = (transf.globalPos.x - transf.size.x / 2) * 2 - 1;
				if (curPos.y < (transf.globalPos.y - transf.size.y / 2) * 2 - 1)
				{
					//std::cout << "Warning: line has been cut" << std::endl;
					break;
				}
			}

			GLfloat xpos = curPos.x + (float)ch.Bearing.x / halfWidth;
			GLfloat ypos = curPos.y;

			GLfloat w = (float)ch.Size.x / halfWidth;
			GLfloat h_m = (float)(ch.Size.y - ch.Bearing.y) / halfHeight;
			GLfloat h_p = (float)ch.Bearing.y / halfHeight;
			auto coords = PixToScreenCoord(ch.Coord, ch.Size);
			std::vector<Vertex2D> vertices = {
				{{xpos, ypos + h_p,	0},	{coords[0].x, coords[0].y}},
				{{xpos, ypos - h_m,	0}, {coords[1].x, coords[1].y}},
				{{xpos + w, ypos - h_m,	0},	{coords[2].x, coords[2].y}},
				{{xpos + w, ypos + h_p,	0}, {coords[3].x, coords[3].y}}
			}; 
			Singleton<SharedGraphicsResources> res;
			static GLMaterial mat(res->GetShaderRef("Shaders\\UiText.ueshad"));
			mat.Textures["text"] = &text.font->texture;
			DrawUI(mat, vertices, transf.priority);
			curPos.x += (float)ch.Advance/ halfWidth;
		}
	}
}

void ui::DrawLine(ecs::EntityManager& em)
{
	for (auto l = em.GetComponents<RenderLine, RectTransform>(); !l.end(); ++l)
	{
		auto [render, tr] = *l;

		glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(tr.size.x, tr.size.y, 1));
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(tr.pos.x * 2 - 1, tr.pos.y * 2 - 1, 1 - (float)tr.priority / 100.0f)) * modelMatrix;
		render.RenderedLine.Draw(render.RenderedMaterial, modelMatrix);
	}
}
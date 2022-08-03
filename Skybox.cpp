#pragma once
#include "Skybox.h"


Skybox::Skybox(GLCubemapTexture map, GLShader& shader): Shader(&shader), CubemapTexture(map)
{

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(SkyboxVertices), &SkyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 * sizeof(GL_FLOAT)), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void Skybox::Draw(glm::mat4 view, glm::mat4 projection)
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	glm::mat4 _view = glm::mat4(glm::mat3(view));

	glUseProgram(Shader->Program);

	glUniformMatrix4fv(glGetUniformLocation(Shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(_view));
	glUniformMatrix4fv(glGetUniformLocation(Shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTexture.Id);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

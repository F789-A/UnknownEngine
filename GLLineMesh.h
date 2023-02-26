#pragma once
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include "GLMaterial.h"

class GLLineMesh
{
public:
	unsigned int VAO;
	unsigned int VBO;
	int Count;
	bool HaveResources = true;

	GLLineMesh()
	{
		HaveResources = false;
	}

	GLLineMesh(const GLLineMesh&) = delete;
	GLLineMesh operator=(const GLLineMesh&) = delete;

	GLLineMesh(GLLineMesh&& other) noexcept
	{
		HaveResources = true;
		other.HaveResources = false;
		VAO = other.VAO;
		VBO = other.VBO;
		Count = other.Count;
	}
	GLLineMesh& operator=(GLLineMesh&& other) noexcept
	{
		if (this != &other)
		{
			HaveResources = true;
			other.HaveResources = false;
			VAO = other.VAO;
			VBO = other.VBO;
			Count = other.Count;
		}
		return *this;
	}

	GLLineMesh(const std::vector<glm::vec2>& arr)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		Count = arr.size();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, arr.size() * sizeof(glm::vec2), &(arr[0]), GL_STREAM_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void setVert(const std::vector<glm::vec2>& arr)
	{
		Count = arr.size();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, arr.size() * sizeof(glm::vec2), &(arr[0]), GL_STREAM_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Draw(const GLMaterial& material)
	{
		material.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, Count);
		glBindVertexArray(0);
	}

	~GLLineMesh()
	{
		if (HaveResources)
		{
			glDeleteVertexArrays(1, &VBO);
			glDeleteBuffers(1, &VAO);
		}
	}
};
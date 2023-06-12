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
	uint EBO;
	int IndicesSize;

	bool HaveResources = true;

	GLLineMesh();
	GLLineMesh(const std::vector<glm::vec2>& vertices, const std::vector<int>& indices);
	GLLineMesh(const GLLineMesh&) = delete;
	GLLineMesh operator=(const GLLineMesh&) = delete;
	GLLineMesh(GLLineMesh&& other) noexcept;
	GLLineMesh& operator=(GLLineMesh&& other) noexcept;
	~GLLineMesh();

	void SetVertices(const std::vector<glm::vec2>& vertices);
	void SetIndices(const std::vector<int>& indices);

	void Draw(const GLMaterial& material);

private:
	int Count;
};
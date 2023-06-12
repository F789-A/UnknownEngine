#include "GLLineMesh.h"

GLLineMesh::GLLineMesh()
{
	HaveResources = false;
}

GLLineMesh::GLLineMesh(GLLineMesh&& other) noexcept
{
	HaveResources = true;
	other.HaveResources = false;
	VAO = other.VAO;
	VBO = other.VBO;
	IndicesSize = other.IndicesSize;

}

GLLineMesh& GLLineMesh::operator=(GLLineMesh&& other) noexcept
{
	if (this != &other)
	{
		HaveResources = true;
		other.HaveResources = false;
		VAO = other.VAO;
		VBO = other.VBO;
		IndicesSize = other.IndicesSize;
	}
	return *this;
}

GLLineMesh::GLLineMesh(const std::vector<glm::vec2>& vertices, const std::vector<int>& indices)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	IndicesSize = vertices.size();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &(vertices[0]), GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesSize * sizeof(int), &(indices[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLLineMesh::SetVertices(const std::vector<glm::vec2>& arr)
{
	Count = arr.size();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	IndicesSize = arr.size();

	glBufferData(GL_ARRAY_BUFFER, arr.size() * sizeof(glm::vec2), &(arr[0]), GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLLineMesh::Draw(const GLMaterial& material)
{
	material.Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, IndicesSize);
	glBindVertexArray(0);
}

GLLineMesh::~GLLineMesh()
{
	if (HaveResources)
	{
		glDeleteVertexArrays(1, &VBO);
		glDeleteBuffers(1, &VAO);
	}
}
#include "model.h"

Model::Model()
{
}

Model::Model(const void *vertices, GLuint vSize, const void *indices, GLuint iSize, GLuint attribIndex, GLenum drawMode)
	: m_attribIndex(attribIndex), m_iSize(iSize)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vSize, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * iSize, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(attribIndex);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// No to - glBindBUffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::setData()
{
}

GLuint Model::getVAO() const noexcept
{
	return m_VAO;
}

GLuint Model::getVBO() const noexcept
{
	return m_VBO;
}

GLuint Model::getEBO() const noexcept
{
	return m_EBO;
}

GLuint Model::getSize() const noexcept
{
	return m_iSize;
}

GLenum Model::getDrawMode() const noexcept
{
	return m_drawMode;
}


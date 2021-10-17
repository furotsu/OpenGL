#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

/*
Class that manages all 3d model data
*/
class Model
{
private:
	std::vector<GLfloat> m_vertices;
	std::vector<GLuint> m_indices;

	bool m_indexed;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_attribIndex;

	GLuint m_Size;

	GLenum m_drawMode;

public:
	Model();
	Model(const void *vertices, GLuint vSize, GLuint attribIndex, GLenum drawMode = GL_TRIANGLES);
	Model(const void *vertices, GLuint vSize, const void *indices, GLuint iSize, GLuint attribIndex, GLenum drawMode = GL_TRIANGLES);
	Model(const Model& p1);

	void setData();

	bool isIndexed() const noexcept;

	GLuint getVAO() const noexcept;
	GLuint getVBO() const noexcept;
	GLuint getEBO() const noexcept;
	GLuint getSize() const noexcept;
	GLenum getDrawMode() const noexcept;

	void changeModel();

};
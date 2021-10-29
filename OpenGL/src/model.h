#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Texture.h"

#include "vendor/glm/glm.hpp"

#include <string>
#include <vector>

/*
Class that manages all 3d model data
*/

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normals;
	glm::vec2 TexCoords;
};

class Model
{
private:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_textures;

	bool m_indexed;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_attribIndex;


	GLuint m_Size;

	GLenum m_drawMode;

public:
	Model();
	//Model(const void *vertices, GLuint vSize, GLuint attribIndex, GLenum drawMode = GL_TRIANGLES);
	//Model(const void* vertices, GLuint vSize, GLuint attribIndex, std::vector<std::string> textureFilePath, GLenum drawMode = GL_TRIANGLES);
	//Model(const void *vertices, GLuint vSize, const void *indices, GLuint iSize, GLuint attribIndex, GLenum drawMode = GL_TRIANGLES);
	Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures); //TODO add pass by reference
	Model(const Model& p1);

	void setData();

	void bindTexture();
	
	bool isIndexed() const noexcept;

	GLuint getVAO() const noexcept;
	GLuint getVBO() const noexcept;
	GLuint getEBO() const noexcept;
	GLuint getSize() const noexcept;
	GLenum getDrawMode() const noexcept;

	void changeModel();

};
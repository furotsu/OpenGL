#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Texture.h"
#include "Shader.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include <string>
#include <vector>

/*
Class that manages all 3d model data
*/

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned short> m_indices;
	std::vector<Texture> m_textures;

	glm::mat4 m_transformMat;

	glm::vec3 m_translation;
	glm::vec4 m_rotation;
	glm::vec3 m_scale;

	bool m_indexed;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_attribIndex;


	GLuint m_Size;

	GLenum m_drawMode;

public:
	Mesh();
	//Mesh(const void *vertices, GLuint vSize, GLuint attribIndex, GLenum drawMode = GL_TRIANGLES);
	//Mesh(const void* vertices, GLuint vSize, GLuint attribIndex, std::vector<std::string> textureFilePath, GLenum drawMode = GL_TRIANGLES);
	//Mesh(const void *vertices, GLuint vSize, const void *indices, GLuint iSize, GLuint attribIndex, GLenum drawMode = GL_TRIANGLES);
	Mesh(std::vector<Vertex> &vertices, std::vector<unsigned short> &indices, std::vector<Texture> &textures, glm::vec3 translation = glm::vec3(1.0f),
														glm::vec4 rotation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3 scale = glm::vec3(1.0f)); //TODO add pass by reference
	Mesh(std::vector<Vertex> &vertices, std::vector<unsigned short> &indices, std::vector<Texture> &textures, glm::mat4 &translation);
	Mesh(const Mesh& p1);

	void initModel();

	void setData();

	void draw(ShaderProgram&);

	void bindTexture();

	bool isIndexed() const noexcept;

	GLuint getVAO() const noexcept;
	GLuint getVBO() const noexcept;
	GLuint getEBO() const noexcept;
	GLuint getSize() const noexcept;
	GLenum getDrawMode() const noexcept;

	void changeModel();
	void move(glm::vec3 position);

};

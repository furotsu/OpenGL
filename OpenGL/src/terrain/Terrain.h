#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Shader.h"
#include "model/Vertex.h"
#include "Texture.h"



class Terrain
{
	unsigned int m_verticesCount;
	unsigned int m_width;
	unsigned int m_length;
	std::vector<Vertex> m_vertices;
	std::vector<int> m_indices;
	Texture m_diffuseTexture;
	std::string m_textureFilepath;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_attribIndex;
public:
	Terrain();
	Terrain(unsigned int, unsigned int, unsigned int, std::string);

	void generateTerrain();
	void initTerrain();
	void draw(ShaderProgram&);
};
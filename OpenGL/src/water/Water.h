#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Shader.h"
#include "actor/Vertex.h"

class Water
{
	unsigned int m_verticesCount;
	unsigned int m_width;
	unsigned int m_length;
	float m_waterLevel;
	float m_vertexSize;
	std::vector<VertexWater> m_vertices;
	std::vector<unsigned short> m_indices;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_attribIndex;

public:
	Water();
	Water(unsigned int, unsigned int, unsigned int, float);

	void generateWater();
	void initWater();
	void draw(ShaderProgram&);

	float getHeight(float, float);
};
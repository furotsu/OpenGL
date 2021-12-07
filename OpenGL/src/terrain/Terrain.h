#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Shader.h"
#include "actor/Vertex.h"
#include "Texture.h"
#include "stb_image/stb_image.h"


constexpr float INVALID_HEIGHT = -666.66666f;


class Terrain
{
	unsigned int m_verticesCount;
	unsigned int m_width;
	unsigned int m_length;
	float m_vertexSize;
	float m_brushSize;
	bool m_isChanging;
	glm::vec2 m_brushPos;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned short> m_indices;
	Texture m_diffuseTexture;
	Texture m_heightMapTexture;
	std::string m_textureFilepath;
	std::string m_heightMapFilepath;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_attribIndex;
public:
	Terrain();
	Terrain(unsigned int, unsigned int, unsigned int, std::string, std::string);
	
	void generateTerrain();
	void initTerrain();
	void draw(ShaderProgram&);
	void setBrushPosition(glm::vec2);
	void increaseHeight();
	void decreaseHeight();

	float getHeight(float, float);

	void saveHeightMap(std::string);
	void loadHeightMap(std::string);
private:
	void updateHeight();
	float colorToHeight(int x, int z, const stbi_uc *pixelsBuf);
	float heightToColor(float);

};
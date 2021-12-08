#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Shader.h"
#include "actor/Vertex.h"
#include "memory"

class WaterFrameBuffer;

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
	std::unique_ptr<WaterFrameBuffer> m_WFB;
	Water();
	Water(unsigned int, unsigned int, unsigned int, float);

	void generateWater();
	void initWater();
	void draw(ShaderProgram&);
	void bindFramebuffer();
	void unbindFramebuffer();
	float getHeight(float, float);

	friend class WaterFrameBuffer;
};

class WaterFrameBuffer
{

	const static int REFLECTION_WIDTH = 256;
	const static int REFLECTION_HEIGHT = 256;

	const static int REFRACTION_WIDTH = 1280;
	const static int REFRACTION_HEIGHT = 720;

	unsigned int m_reflectionFbo;
	unsigned int m_reflectionDepthBuffer;

	unsigned int m_reflectionRbo;

	//int m_refractionFrameBuffer;
	//int m_refractionTexture;
	//int m_refractionDepthTexture;

public:
	unsigned int m_reflectionTexture;
	WaterFrameBuffer();
	~WaterFrameBuffer();

	void cleanUp();
	void bindReflectionFrameBuffer();
	//void bindRefractionFrameBuffer();
	void unbindCurrentFrameBuffer();

	int getReflectionTexture();
	//int getRefractionTexture();
	//int getRefractionDepthTexture();

private:
	void initialiseReflectionFrameBuffer();
	//void initialiseRefractionFrameBuffer();
	void bindFrameBuffer(int frameBuffer, int width, int height);
	//void createFrameBuffer();
	//void createTextureAttachment();
	//void createDepthStencilBufferAttachment();
};
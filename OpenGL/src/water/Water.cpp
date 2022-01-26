#include "Water.h"
#include "debugger.h"

Water::Water()
	: m_verticesCount(128), m_width(800), m_length(800), m_waterLevel(0.0f)
{
	m_vertexSize = m_width / ((float)m_verticesCount - 1);
	generateWater();
	m_WFB = std::make_unique<WaterFrameBuffer>();
	m_modelMat = glm::mat4(1.0f);
	m_modelMat = glm::translate(m_modelMat, glm::vec3(0.0f, m_waterLevel, 0.0f));
}

Water::Water(unsigned int verticesCount, unsigned int width, unsigned int length, float waterLevel)
	: m_verticesCount(verticesCount), m_width(width), m_length(length), m_waterLevel(waterLevel)
{
	m_vertexSize = m_width / ((float)m_verticesCount - 1);
	generateWater();
	initWater();
	m_WFB = std::make_unique<WaterFrameBuffer>();
	m_modelMat = glm::mat4(1.0f);
	m_modelMat = glm::translate(m_modelMat, glm::vec3(0.0f, m_waterLevel, 0.0f));
}

void Water::generateWater()
{
	int count = m_verticesCount * m_verticesCount;
	std::vector<float> v1;

	VertexWater v;
	for (int i = 0; i < m_verticesCount; i++)
	{
		for (int j = 0; j < m_verticesCount; j++)
		{
			v.Position.x = (float)i * m_width / ((float)m_verticesCount - 1.0f) - m_width / 2;
			v.Position.y = m_waterLevel;
			v.Position.z = (float)j * m_length / ((float)m_verticesCount - 1.0f) - m_length / 2;

			v.Color.x = 44.0f;
			v.Color.y = 208.0f;
			v.Color.z = 226.0f;


			m_vertices.push_back(v);
		}
	}

	for (int gz = 0; gz < m_verticesCount - 1; gz++) {
		for (int gx = 0; gx < m_verticesCount - 1; gx++) {
			int topLeft = gz + (gx * m_verticesCount);
			int topRight = topLeft + 1;
			int bottomLeft = topLeft + m_verticesCount;
			int bottomRight = bottomLeft + 1;
			m_indices.push_back(topLeft);
			m_indices.push_back(bottomLeft);
			m_indices.push_back(topRight);
			m_indices.push_back(topRight);
			m_indices.push_back(bottomLeft);
			m_indices.push_back(bottomRight);
		}
	}
}

void Water::initWater()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(VertexWater), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned short), &m_indices[0], GL_STATIC_DRAW);

	//vertices positions
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWater), (void*)0); // TODO - somehow automatize attribIndex setup
	glEnableVertexAttribArray(1);

	//color values
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWater), (void*)offsetof(VertexWater, Color));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Water::draw(ShaderProgram& program)
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

void Water::drawNormals(std::shared_ptr<ShaderProgram>)
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

void Water::bindFramebuffer()
{
	m_WFB->bindReflectionFrameBuffer();
	//m_WFB->bindRefractionFrameBuffer();
}

void Water::unbindFramebuffer()
{
	m_WFB->unbindCurrentFrameBuffer();
}

//TODO
float Water::getHeight(float, float)
{
	return m_waterLevel;
}

float Water::getWaterLevel()
{
	return m_waterLevel;
}

glm::mat4 Water::getModelMat()
{
	return m_modelMat;
}

WaterFrameBuffer::WaterFrameBuffer()
{
	initialiseReflectionFrameBuffer();
	initialiseRefractionFrameBuffer();
}

WaterFrameBuffer::~WaterFrameBuffer()
{
	unbindCurrentFrameBuffer();
	cleanUp();
}

void WaterFrameBuffer::cleanUp()
{
	glDeleteFramebuffers(1, &m_reflectionFbo);
	glDeleteTextures(1, &m_reflectionTexture);
	glDeleteRenderbuffers(1, &m_reflectionRbo);

	glDeleteFramebuffers(1, &m_refractionFbo);
	glDeleteTextures(1, &m_refractionTexture);
	glDeleteRenderbuffers(1, &m_refractionRbo);
}

void WaterFrameBuffer::bindReflectionFrameBuffer()
{
	bindFrameBuffer(m_reflectionFbo, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}

void WaterFrameBuffer::bindRefractionFrameBuffer()
{
	bindFrameBuffer(m_refractionFbo, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}

void WaterFrameBuffer::bindFrameBuffer(int frameBuffer, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

void WaterFrameBuffer::unbindCurrentFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1600, 900);
}

int WaterFrameBuffer::getReflectionTexture()
{
	return m_reflectionTexture;
}

int WaterFrameBuffer::getRefractionTexture()
{
	return m_refractionTexture;
}

void WaterFrameBuffer::initialiseReflectionFrameBuffer()
{
	//Frame buffer object color texture
	glGenFramebuffers(1, &m_reflectionFbo);
	glGenTextures(1, &m_reflectionTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, m_reflectionFbo);
	glBindTexture(GL_TEXTURE_2D, m_reflectionTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, REFLECTION_WIDTH, REFLECTION_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_reflectionTexture, 0);

	//Render buffer object for depth and stencil buffers
	glGenRenderbuffers(1, &m_reflectionRbo);

	glBindRenderbuffer(GL_RENDERBUFFER, m_reflectionRbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, REFLECTION_WIDTH, REFLECTION_HEIGHT);

	//attach renderbuffer to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_reflectionRbo);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ERROR("WATER FRAMEBUFFER: reflection framebuffer is not complete");
	}

	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WaterFrameBuffer::initialiseRefractionFrameBuffer()
{
	//Frame buffer object color texture
	glGenFramebuffers(1, &m_refractionFbo);
	glGenTextures(1, &m_refractionTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, m_refractionFbo);
	glBindTexture(GL_TEXTURE_2D, m_refractionTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, REFRACTION_WIDTH, REFRACTION_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_refractionTexture, 0);

	//Render buffer object for depth and stencil buffers
	glGenRenderbuffers(1, &m_refractionRbo);

	glBindRenderbuffer(GL_RENDERBUFFER, m_refractionRbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, REFRACTION_WIDTH, REFRACTION_HEIGHT);

	//attach renderbuffer to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_refractionRbo);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ERROR("WATER FRAMEBUFFER: refraction framebuffer is not complete");
	}

	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 1);
}




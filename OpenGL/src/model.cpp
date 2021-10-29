#include "model.h"
#include "vendor/stb_image/stb_image.h"

#include <iostream>
#include <utility>

Model::Model()
{
}

Model::Model(const Model& p1)
	:m_attribIndex(p1.m_attribIndex), m_Size(p1.m_Size), m_indexed(p1.m_indexed), m_drawMode(p1.m_drawMode)
{
	if (p1.getDrawMode())
	{
		//TODO
	}
	else
	{
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		// We only need to bind to the VBO, the container's VBO's data already contains the correct data.
		glBindBuffer(GL_ARRAY_BUFFER, p1.m_VBO);
		
		glVertexAttribPointer(m_attribIndex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(m_attribIndex);
	}
}



void Model::setData()
{
}

void Model::bindTexture()
{
	//glBindTexture(GL_TEXTURE_2D, 1);
	for (unsigned int i = 0; i != m_textures.size(); ++i)
	{
		m_textures[i].Bind(i);
	}
}

bool Model::isIndexed() const noexcept
{
	return m_indexed;
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
	return m_Size;
}

GLenum Model::getDrawMode() const noexcept
{
	return m_drawMode;
}

void Model::changeModel()
{
	// some stuff with GLmap and so on
}

/*
Model::Model(const void *vertices, GLuint vSize, GLuint attribIndex, GLenum drawMode)
	:m_attribIndex(attribIndex), m_Size(vSize), m_indexed(GL_FALSE), m_drawMode(drawMode), m_EBO(0)
{

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vSize * 8, vertices, GL_STATIC_DRAW);
	std::cout << sizeof(float) * vSize << std::endl;
	glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(attribIndex);

	//normal values
	glVertexAttribPointer(attribIndex + 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(attribIndex + 1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}
Model::Model(const void *vertices, GLuint vSize, GLuint attribIndex, std::vector<std::string> textureFileName, GLenum drawMode)
	:m_attribIndex(attribIndex), m_Size(vSize), m_indexed(GL_FALSE), m_drawMode(drawMode), m_EBO(0)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vSize * 8, vertices, GL_STATIC_DRAW);
	std::cout << sizeof(float) * vSize << std::endl;
	glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(attribIndex);

	//normal values
	glVertexAttribPointer(attribIndex + 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(attribIndex + 1);

	// texture coords
	glVertexAttribPointer(attribIndex + 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(attribIndex + 2);

	for (auto& fileName : textureFileName)
	{
		m_textures.push_back(std::move(Texture(fileName)));
	}
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

Model::Model(const void *vertices, GLuint vSize, const void *indices, GLuint iSize, GLuint attribIndex, GLenum drawMode)
	: m_attribIndex(attribIndex), m_Size(iSize), m_indexed(GL_TRUE), m_drawMode(drawMode)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vSize * 3, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * iSize, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(attribIndex);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// No to - glBindBUffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
*/



#include "Mesh.h"
#include "vendor/stb_image/stb_image.h"
#include <glm/gtx/matrix_decompose.hpp>

#include <iostream>
#include <vector>
#include <utility>

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned short> &indices, std::vector<std::shared_ptr<Texture>> &textures, glm::vec3 translation, glm::vec4 rotation, glm::vec3 scale)
	: m_indexed(true)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;

	m_transformMat = glm::mat4(1.0f);
	m_transformMat = glm::translate(m_transformMat, translation);
	m_transformMat = glm::rotate(m_transformMat, rotation.z, glm::vec3(rotation.x, rotation.y, rotation.z));
	m_transformMat = glm::scale(m_transformMat, scale);

	m_position = translation;
	m_translation = translation;
	m_rotation = rotation;
	m_scale = scale;

	initModel();
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned short>& indices, std::vector<std::shared_ptr<Texture>>& textures, glm::mat4& translation)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;

	m_transformMat = translation;
	glm::vec3 a;
	glm::quat q;
	glm::vec4 b;
	glm::decompose(m_transformMat, m_scale, q, m_translation, a, b);
	m_position = m_translation;

	initModel();
}


Mesh::Mesh(const Mesh& p1)
	:m_attribIndex(p1.m_attribIndex), m_Size(p1.m_Size), m_indexed(p1.m_indexed), m_drawMode(p1.m_drawMode)
{
	m_vertices = p1.m_vertices;
	m_indices =  p1.m_indices;
	m_textures = p1.m_textures;

	m_transformMat = glm::mat4(1.0f);
	m_transformMat = glm::translate(m_transformMat, p1.m_translation);
	m_transformMat = glm::rotate(m_transformMat, glm::radians(glm::degrees(p1.m_rotation.z)), glm::vec3(p1.m_rotation.x, p1.m_rotation.y, p1.m_rotation.z));
	m_transformMat = glm::scale(m_transformMat, p1.m_scale);

	m_position = m_translation;
	m_translation = p1.m_translation;
	m_rotation =	p1.m_rotation;
	m_scale =		p1.m_scale;

	initModel();
}

void Mesh::initModel()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned short), &m_indices[0], GL_STATIC_DRAW);

	//vertices positions
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // TODO - somehow automatize attribIndex setup
	glEnableVertexAttribArray(1);
	//normal values
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);

	// texture coords
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw(ShaderProgram& program)
{

	//program.SetUniform1f("material.shininess", 64.0f);
	glActiveTexture(GL_TEXTURE0);

	program.SetUniformMat4f("model", m_transformMat);	

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

void Mesh::setTextureUniform(ShaderProgram& program)
{
	unsigned int diffuseNum = 1;
	unsigned int specularNum = 1;
	std::string name;

	for (unsigned int i = 0; i != m_textures.size(); ++i)
	{

		std::string number;
		TextureType textureType = m_textures[i]->type;

		switch (textureType)
		{
		case TextureType::Diffuse: number = std::to_string(diffuseNum++); name = "material.texture_diffuse"; break;
		case TextureType::Specular: number = std::to_string(specularNum++); name = "material.texture_specular"; break;
		default: std::cout << "O_o texture type troubles in model drawing function" << std::endl;
		}
		program.SetUniform1f((name + number).c_str(), i);

		m_textures[i]->Bind(i);
	}
}

void Mesh::setData()
{
}

// TODO delete this?
void Mesh::bindTexture()
{
	//glBindTexture(GL_TEXTURE_2D, 1);
	for (unsigned int i = 0; i != m_textures.size(); ++i)
	{
		m_textures[i]->Bind(i);
	}
}

bool Mesh::isIndexed() const noexcept
{
	return m_indexed;
}

GLuint Mesh::getVAO() const noexcept
{
	return m_VAO;
}

GLuint Mesh::getVBO() const noexcept
{
	return m_VBO;
}

GLuint Mesh::getEBO() const noexcept
{
	return m_EBO;
}

GLuint Mesh::getSize() const noexcept
{
	return m_Size;
}

GLenum Mesh::getDrawMode() const noexcept
{
	return m_drawMode;
}

void Mesh::changeModel()
{
	// some stuff with GLmap and so on
}

void Mesh::move(glm::vec3 &translation)
{
	m_transformMat = glm::translate(m_transformMat, translation);
	m_translation += translation - 1.0f;
}

void Mesh::rotate(glm::vec4& rotation)
{
	//m_transformMat = glm::rotate(m_transformMat, glm::radians(glm::degrees(rotation[3])), glm::vec3(rotation.x, rotation.y, rotation.z));
}

void Mesh::scale(glm::vec3& scale)
{
	m_transformMat = glm::scale(m_transformMat, scale);
	m_scale += scale;
	//m_translation 
	//m_transformMat = glm::translate(m_transformMat, m_translation / (scale/2.0f));
}



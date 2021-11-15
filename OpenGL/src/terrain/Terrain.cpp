#include "Terrain.h"

Terrain::Terrain()
	: m_verticesCount(128), m_width(800), m_length(800), m_textureFilepath("res/textures/cobblestone.jpg")
{
	generateTerrain();
	m_diffuseTexture = Texture(m_textureFilepath, TextureType::Diffuse);
}

Terrain::Terrain(unsigned int verticesCount, unsigned int width, unsigned int length, std::string filepath)
	: m_verticesCount(verticesCount), m_width(width), m_length(length), m_textureFilepath(filepath)
{
	generateTerrain();
	initTerrain();
	m_diffuseTexture = Texture(m_textureFilepath, TextureType::Diffuse);
}

void Terrain::generateTerrain()
{
	int count = m_verticesCount * m_verticesCount;
	Vertex v;
	for (int i = 0; i < m_verticesCount; i++) {
		for (int j = 0; j < m_verticesCount; j++) {
			v.Position.x = (float)i * m_width / ((float)m_verticesCount - 1.0f);
			v.Position.y = 0;
			v.Position.z = (float)j * m_length / ((float)m_verticesCount - 1.0f);

			v.Normal.x = 0;
			v.Normal.y = 1;
			v.Normal.z = 0;

			v.TexCoords.x = (float)i / ((float)m_verticesCount - 1.0f);
			v.TexCoords.y = (float)j / ((float)m_verticesCount - 1.0f);

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

void Terrain::initTerrain()
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

void Terrain::draw(ShaderProgram& program)
{
	program.SetUniform1f("texture_diffuse", m_diffuseTexture.m_id);
	m_diffuseTexture.Bind();

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

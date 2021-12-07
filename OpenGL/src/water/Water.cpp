#include "Water.h"

Water::Water()
	: m_verticesCount(128), m_width(800), m_length(800), m_waterLevel(0.0f)
{
	m_vertexSize = m_width / ((float)m_verticesCount - 1);
	generateWater();
}

Water::Water(unsigned int verticesCount, unsigned int width, unsigned int length, float waterLevel)
	: m_verticesCount(verticesCount), m_width(width), m_length(length), m_waterLevel(waterLevel)
{
	m_vertexSize = m_width / ((float)m_verticesCount - 1);
	generateWater();
	initWater();
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

//TODO
float Water::getHeight(float, float)
{
	return m_waterLevel;
}

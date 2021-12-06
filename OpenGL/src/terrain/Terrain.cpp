#include "Terrain.h"
#include "math/math.h"
#include "debugger.h"

Terrain::Terrain()
	: m_verticesCount(128), m_width(800), m_length(800), m_textureFilepath("res/textures/cobblestone.jpg")
{
	m_vertexSize = m_width / ((float)m_verticesCount - 1);
	generateTerrain();
	m_diffuseTexture = Texture(m_textureFilepath, TextureType::Diffuse);
}

Terrain::Terrain(unsigned int verticesCount, unsigned int width, unsigned int length, std::string filepath, std::string heightMap)
	: m_verticesCount(verticesCount), m_width(width), m_length(length), m_textureFilepath(filepath), m_heightMapFilepath(heightMap)
{
	m_brushPos = glm::vec2(0.0f, 0.0f);
	m_brushSize = 1.0f;
	m_isChanging = false;
		m_vertexSize = m_width / ((float)m_verticesCount - 1);
	generateTerrain();
	initTerrain();
	m_diffuseTexture = Texture(m_textureFilepath, TextureType::Diffuse);
}

void Terrain::generateTerrain()
{
	int count = m_verticesCount * m_verticesCount;
	std::vector<float> v1;
	//math::genRanN(count, 100, v1);

	int BPP;
	int width, height;
	stbi_uc* buffer = stbi_load(m_heightMapFilepath.c_str(), &width, &height, &BPP, 0);

	if (buffer)
	{
		Vertex v;
		for (int i = 0; i < m_verticesCount; i++) 
		{
			for (int j = 0; j < m_verticesCount; j++) 
			{
				v.Position.x = (float)i * m_width / ((float)m_verticesCount - 1.0f) - m_width / 2;
				v.Position.y = getHeight(i, j, buffer);//v1[i*m_verticesCount + j]/(float)2;
				v.Position.z = (float)j * m_length / ((float)m_verticesCount - 1.0f) - m_length / 2;

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
	else
	{
		ASSERT(false && "Cannot read height map file");
	}
	stbi_image_free(buffer);
}

float Terrain::getHeight(int x, int z, const stbi_uc *buffer)
{
	if (x < 0 || x > 256 || z < 0 || z > 256)
	{
		return 0;
	}
	const stbi_uc* pixelOffset = buffer + (3 * (z * m_width + x))*sizeof(unsigned char);
	stbi_uc r = pixelOffset[0];
	float height = (static_cast<float>(r) + 256.0f) / 256.0f;
	return height;
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
	program.SetUniform1f("isChanging", (m_isChanging) ? 1.0f : 0.0f);

	m_diffuseTexture.Bind();

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	if (m_isChanging)
	{
		program.SetUniform2f("circlePosition", m_brushPos.x, m_brushPos.y);
		program.SetUniform1f("brushSize", m_brushSize);
	}

}

void Terrain::setBrushPosition(glm::vec2 brushPosition)
{
	m_isChanging = true;
	m_brushPos = brushPosition;
}

void Terrain::increaseHeight()
{
	float x = m_brushPos.x;
	float z = m_brushPos.y;
	x += m_width / 2;
	z += m_length / 2;
	x /= m_vertexSize;
	z /= m_vertexSize;
	m_vertices[(int)(x + z * m_verticesCount)].Position.y += 0.001;

}

float Terrain::getHeight(float x, float	z)
{
	x += m_width / 2;
	z += m_length / 2;
	if (x < 0.0 || x > 8.0 || z < 0.0 || z > 8.0)
	{
		return INVALID_HEIGHT;
	}
	else
	{
		x /= m_vertexSize;
		z /= m_vertexSize;
	}
	//std::cout << "x: " << x << " z: " << z << std::endl;
	//std::cout << (int)(x + z * m_verticesCount) << std::endl;
	return m_vertices[(int)(x + z*m_verticesCount)].Position.y;
}


#include "Terrain.h"
#include "math/math.h"
#include "debugger.h"


#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include "stb_image/stb_image_write.h"

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


	loadHeightMap(m_heightMapFilepath);

	/*
	* TODO - have  to got possibility to generate blank terrain
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
	*/

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

	//texture coords
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
	float z	 = m_brushPos.y;
	int i = (x + m_width / 2) * ((float)m_verticesCount - 1.0f)/m_width;
	int j = (z + m_length / 2) * ((float)m_verticesCount - 1.0f) / m_length;
	int brushSize = m_brushSize * ((float)m_verticesCount - 1.0f) / m_width;
	float level = m_brushSize/10.f;
	for (int k = 0; k != brushSize + 1; ++k)
	{
		level -= m_brushSize / (10.f * static_cast<float>(brushSize));
		// set right row around center point on level k
		for (int l = -k; l < k + 1; ++l)
			if ((j + k) < m_verticesCount - 1 && (j + k) > 0 && (i + l) < m_verticesCount - 1 && (i + l) > 0)
				m_vertices[(int)((j + k) + (i + l) * m_verticesCount)].Position.y += level;
		// for bottom
		for (int l = -k; l < k; ++l)
			if ((j + l) > 0 && (i + l) < m_verticesCount - 1 && (i + k) > 0 && (i + k) < m_verticesCount - 1)
				m_vertices[(int)((j + l) + (i + k) * m_verticesCount)].Position.y += level;
		// for left
		for (int l = -k; l < k; ++l)
			if ((j - k) > 0 && (j - k) < m_verticesCount - 1 && (i + l) < m_verticesCount - 1 && (i + l) > 0)
				m_vertices[(int)((j - k) + (i + l) * m_verticesCount)].Position.y += level;
		// for top
		for (int l = -k + 1; l < k; ++l)
			if ((j + l) < m_verticesCount - 1 && (j + l) > 0 && (i - k) > 0 && (i - k) < m_verticesCount - 1)
				m_vertices[(int)((j + l) + (i - k) * m_verticesCount)].Position.y += level;
	}

	//if (j < m_verticesCount - 1 && j > 0 &&)
	//m_vertices[(int)(j + i * m_verticesCount)].Position.y += m_brushSize / 10.f;

	updateHeight();
}

void Terrain::decreaseHeight()
{
	float x = m_brushPos.x;
	float z = m_brushPos.y;
	int i = (x + m_width / 2) * ((float)m_verticesCount - 1.0f) / m_width;
	int j = (z + m_length / 2) * ((float)m_verticesCount - 1.0f) / m_length;
	int brushSize = m_brushSize * ((float)m_verticesCount - 1.0f) / m_width;
	float level = m_brushSize / 10.f;
	for (int k = 0; k != brushSize + 1; ++k)
	{
		level -= m_brushSize / (10.f * static_cast<float>(brushSize));
		// set right row around center point on level k
		for (int l = -k; l < k + 1; ++l)
			if ((j + k) < m_verticesCount - 1 && (j + k) > 0 && (i + l) < m_verticesCount - 1 && (i + l) > 0)
				m_vertices[(int)((j + k) + (i + l) * m_verticesCount)].Position.y -= level;
		// for bottom
		for (int l = -k; l < k; ++l)
			if ((j + l) > 0 && (i + l) < m_verticesCount - 1 && (i + k) > 0 && (i + k) < m_verticesCount - 1)
				m_vertices[(int)((j + l) + (i + k) * m_verticesCount)].Position.y -= level;
		// for left
		for (int l = -k; l < k; ++l)
			if ((j - k) > 0 && (j - k) < m_verticesCount - 1 && (i + l) < m_verticesCount - 1 && (i + l) > 0)
				m_vertices[(int)((j - k) + (i + l) * m_verticesCount)].Position.y -= level;
		// for top
		for (int l = -k + 1; l < k; ++l)
			if ((j + l) < m_verticesCount - 1 && (j + l) > 0 && (i - k) > 0 && (i - k) < m_verticesCount - 1)
				m_vertices[(int)((j + l) + (i - k) * m_verticesCount)].Position.y -= level;
	}

	updateHeight();
}

float Terrain::getHeight(float x, float	z)
{
	x += m_width / 2;
	z += m_length / 2;
	if (x < 0.0)
		x = 0.0f;
	else if (x > 8.0)
		x = 8.0f;

	if (z < 0.0)
		z = 0.0f;
	else if (z > 8.0)
		z = 8.0f;

	int i = x * ((float)m_verticesCount - 1.0f) / m_width;
	int j = z * ((float)m_verticesCount - 1.0f) / m_length;

	return m_vertices[(int)(x + z*m_verticesCount)].Position.y;
}

void Terrain::updateHeight()
{
	glDeleteVertexArrays(1, &m_VAO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	//vertices positions
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // TODO - somehow automatize attribIndex setup
	glEnableVertexAttribArray(1);
	//normal values
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);

	// texture coords
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(3);

}

void Terrain::saveHeightMap(std::string filename)
{
	uint32_t width = static_cast<uint32_t>(m_verticesCount);
	uint32_t height = static_cast<uint32_t>(m_verticesCount);
	int chanelAmount = 3;

	uint8_t* pixelsBuf = (uint8_t*)malloc(width * height * chanelAmount);

	if (pixelsBuf)
	{
		unsigned int index = 0;
		for (int i = 0; i != m_verticesCount; ++i)
		{
			for (int j = 0; j != m_verticesCount; ++j)
			{
				// since it needs to be black and white writing rgb as same value
				pixelsBuf[index++] = heightToColor(m_vertices[i + j * m_verticesCount].Position.y);
				pixelsBuf[index++] = heightToColor(m_vertices[i + j * m_verticesCount].Position.y);
				pixelsBuf[index++] = heightToColor(m_vertices[i + j * m_verticesCount].Position.y);
			}
		}

		stbi_write_png(filename.c_str(), width, height, chanelAmount, pixelsBuf, width * chanelAmount);
	}
	else
	{
		ERROR("Cannot allocate memory block for pixels buffer trying to save height map into image.");
	}
	free(pixelsBuf);
}

void Terrain::loadHeightMap(std::string filepath)
{

	int BPP;
	int width, height;

	uint8_t* pixelsBuf = stbi_load(m_heightMapFilepath.c_str(), &width, &height, &BPP, 0);

	if (pixelsBuf)
	{
		Vertex v;
		for (int i = 0; i < m_verticesCount; i++)
		{
			for (int j = 0; j < m_verticesCount; j++)
			{
				v.Position.x = (float)i * m_width / ((float)m_verticesCount - 1.0f) - m_width / 2;
				v.Position.y = colorToHeight(j, i, pixelsBuf);
				v.Position.z = (float)j * m_length / ((float)m_verticesCount - 1.0f) - m_length / 2;

				v.Normal.x = 0;
				v.Normal.y = 1;
				v.Normal.z = 0;

				v.TexCoords.x = (float)i / ((float)m_verticesCount - 1.0f);
				v.TexCoords.y = (float)j / ((float)m_verticesCount - 1.0f);

				m_vertices.push_back(v);
			}
		}
	}
	else
	{
		ERROR("Cannot read height map file");
	}

	stbi_image_free(pixelsBuf);
}

float Terrain::heightToColor(float height)
{
	return -(height + 5.0f) * 10;
}

float Terrain::colorToHeight(int x, int z, const stbi_uc* pixelsBuf)
{
	if (x < 0 || x > 256 || z < 0 || z > 256)
	{
		return 0;
	}
	const stbi_uc* pixelOffset = pixelsBuf + (3 * (x * m_verticesCount + z));
	stbi_uc r = pixelOffset[0];
	float height = (static_cast<float>(r*10) + 256.0f) / 256.0f - 5.0f;
	return -height;
}


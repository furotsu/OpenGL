#include "Texture.h"
#include "vendor/stb_image/stb_image.h"
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <iostream>
/*
bool TextureLoader::VLoadResource(char* rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> handle)
{
	return false;
}
*/
Texture::Texture() {}
Texture::Texture(const std::string& filepath, TextureType type)
	: m_id(0), m_LocalBuffer(nullptr), type(type),
		m_Width(0), m_Height(0), m_BPP(0)
{
	glGenTextures(1, &m_id);

	//stbi_set_flip_vertically_on_load(1);

	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 0);

    if (m_LocalBuffer)
    {
		GLenum format;
		if (m_BPP == 1)
			format = GL_RED;
		else if (m_BPP == 3)
			format = GL_RGB;
		else if (m_BPP == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(m_LocalBuffer);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
		stbi_image_free(m_LocalBuffer);
    }

}

Texture::~Texture()
{
	//glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


#pragma once

#include <string>

enum class TextureType
{
	Diffuse,
	Specular
};

class Texture
{
private:
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	unsigned int m_id;
	TextureType type;
	std::string filepath;


	Texture();
	Texture(const std::string& filepath, TextureType type);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

};
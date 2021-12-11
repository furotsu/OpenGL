#pragma once

#include <string>
#include <vector>

#include "Interfaces.h"

enum class TextureType
{
	Diffuse,
	Specular,
	CubeMap
};
/*
class TextureLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() override { return false; }
	virtual bool VDiscardRawBufferAfterLoad() override { return true; }
	virtual unsigned int VGetLoadedResourceSize(char* rawBuffer, unsigned int rawSize) override { return 0; }
	virtual bool VLoadResource(char* rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> handle) override;
};
*/
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
	Texture(const std::vector<std::string>& faces, TextureType type = TextureType::CubeMap);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

};
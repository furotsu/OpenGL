#pragma once

#include <GL/glew.h>

#include <string>
#include <map>
#include <vector>

typedef struct
{
	GLenum type;
	const char* filePath;
	GLuint shaderID;
} ShaderInfo;

class ShaderProgram
{
private:
	unsigned int m_programID;
	std::string m_shaderSource;
	std::map<std::string, int> m_uniformLocation;

public:
	ShaderProgram();
	ShaderProgram(std::vector<ShaderInfo> &shaders);
	~ShaderProgram();

	void Bind() const;
	void Unbind() const;

private:
	const std::string parseShader(std::string filePath); //TODO how not to copy local string??
	GLuint compileShader(const unsigned int &type, const std::string &source);
};

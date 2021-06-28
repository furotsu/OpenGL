#pragma once

#include <GL/glew.h>

#include <string>
#include <map>
#include <vector>

typedef struct
{
	GLenum type;
	std::string filePath;
	GLuint shaderID;
} ShaderInfo;

class ShaderProgram
{
public: // CHANGE
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
	GLuint compileShader(GLenum &type, const std::string &source);
};

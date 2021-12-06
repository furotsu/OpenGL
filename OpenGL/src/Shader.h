#pragma once

#include <GL/glew.h>
#include "vendor/glm/glm.hpp"

#include <string>
#include <map>
#include <vector>
#include <sstream>

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
public:
	void SetUniform1i(const std::string&, GLint);
	void SetUniform1f(const std::string&, GLfloat);
	void SetUniform2f(const std::string&, GLfloat, GLfloat);
	void SetUniform3f(const std::string&, GLfloat, GLfloat, GLfloat);
	void SetUniform4f(const std::string&, GLfloat, GLfloat, GLfloat, GLfloat);
	void SetUniformMat4f(const std::string&, glm::mat4&);

	//void drawUniforms();

private:
	GLint getUniformLocation(const std::string& name);

};

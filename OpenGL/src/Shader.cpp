#include "Shader.h"

#include <fstream>
#include <iostream>

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::ShaderProgram(std::vector<ShaderInfo> &shaders)
{
	m_programID = glCreateProgram();


	for (ShaderInfo &shaderEntry : shaders)
	{
		shaderEntry.shaderID = compileShader(shaderEntry.type, parseShader(shaderEntry.filePath));

		glAttachShader(m_programID, shaderEntry.shaderID);

	}

	glLinkProgram(m_programID);
	glValidateProgram(m_programID);

	int success{ 0 };
#ifdef _DEBUG
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		std::cout << "Not linked " << std::endl; //TODO
	}
#endif //DEBUG

	for (ShaderInfo& shaderEntry : shaders)
	{
		glDeleteShader(shaderEntry.shaderID);
		shaderEntry.shaderID = 0;
	}
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::Bind() const
{
	glUseProgram(m_programID);
}

void ShaderProgram::Unbind() const
{
	glUseProgram(0);
}

const std::string ShaderProgram::parseShader(std::string filePath)
{
	std::ifstream file(filePath);

	std::string result;

#ifdef _DEBUG
	if (!file)
	{
		std::cout << "cannot parse some shader" << std::endl; //TODO
	}
#endif // DEBUG

	std::string line;
	while (std::getline(file, line))
	{
		result = result + line + "\n";
	}

	std::cout << result;

	file.close();
	
	return result;
}

GLuint ShaderProgram::compileShader(GLenum &type, const std::string& source)
{
	GLuint shader = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	GLint result{ 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
#ifdef _DEBUG
		// TODO
#endif // DEBUG
	}
	return shader;
}

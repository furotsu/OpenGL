#include "Shader.h"

#include <fstream>
#include <iostream>

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::ShaderProgram(std::vector<ShaderInfo> &shaders)
{
	GLuint program = glCreateProgram();


	for (ShaderInfo &shaderEntry : shaders)
	{
		shaderEntry.shaderID = compileShader(shaderEntry.type, parseShader(shaderEntry.filePath));

		glAttachShader(program, shaderEntry.shaderID);

	}

	glLinkProgram(program);
	glValidateProgram(program);

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

	if (!file)
	{
#ifdef _DEBUG
		std::cout << "cannot parse some shader" << std::endl; //TODO
#endif // DEBUG
	}

	std::string line;
	while (std::getline(file, line))
	{
		result = result + line + "\n";
	}
	
	return result;
}

GLuint ShaderProgram::compileShader(const unsigned int& type, const std::string& source)
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

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
		std::cout << "Not linked " << std::endl;
		GLint length{ 0 };
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		glGetProgramInfoLog(m_programID, length, &length, message);
		std::cout << message << std::endl;
		
		//glDeleteProgram(m_programID);
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

#ifdef _DEBUG
	if (result == GL_FALSE)
	{
		GLint length{ 0 };
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(shader, length, &length, message);
		std::cout << message << std::endl;

		glDeleteShader(shader);
		return 0;
	}
#endif // DEBUG
	return shader;
}

void ShaderProgram::SetUniform1i(const std::string& name, GLint value)
{
	glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::SetUniform1f(const std::string& name, GLfloat value)
{
	glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::SetUniform3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2)
{
	glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void ShaderProgram::SetUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void ShaderProgram::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}
/*
void ShaderProgram::DrawUniforms()
{
	GLint count;
	GLint i;
	const GLsizei bufSize = 16;
	GLchar name1[bufSize];
	GLsizei length;
	GLint size;
	GLenum type;
	glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORMS, &count);
	std::cout << "Active shaders: " << count << std::endl;
	for (int i = 0; i < count; i++)
	{
		glGetActiveUniform(m_RendererID, (GLuint)i, bufSize, &length, &size, &type, name1);

		std::cout << i << "   " << type << "  " << name1 << " | " << std::endl;

	}
}
*/
GLint ShaderProgram::getUniformLocation(const std::string& name)
{
	if (m_uniformLocation.find(name) != m_uniformLocation.end())
		return m_uniformLocation[name];

	int location = glGetUniformLocation(m_programID, name.c_str());
	if (location == -1)
		std::cout << "Location allocation error lol: " << name << std::endl;

	m_uniformLocation[name] = location;
	return location;
}



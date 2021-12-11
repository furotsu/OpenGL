#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Texture.h"

class Skybox
{
private:
	GLuint m_VAO;
	GLuint m_VBO;
	Texture m_texture;

	Skybox();
public:
	Skybox(std::vector<std::string>&);

	void draw(ShaderProgram&);

private:
	void init(std::vector<std::string>&);
};
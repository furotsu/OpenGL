#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "model.h"
//#include "shapes.h"

class Renderer
{
private:
	ShaderProgram m_program;

public:
	Renderer();

	void draw(ShaderProgram &program, Model &model) const;
	

};
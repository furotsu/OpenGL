#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "model/model.h"
#include "Camera.h"
#include "lightSource/lightSource.h"
//#include "shapes.h"

class Renderer
{
private:
	ShaderProgram m_program;
	int m_windowWidth;
	int m_windowHeight;
	glm::mat4 m_projMat;

public:
	Renderer(int windowWidth = 1000, int windowHeight = 1000, float fovDegrees = 45.0f);

	// Set all uniforms and make draw calls
	void draw(ShaderProgram &program, std::shared_ptr<Model> model, std::vector<std::shared_ptr<LightSource>> &lightSources, Camera &camera);
};
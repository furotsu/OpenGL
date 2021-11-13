#pragma once

#include "vendor/gui/imgui.h"
#include "vendor/gui/imgui_impl_glfw_gl3.h"

#include "lightSource/lightSource.h"

class Gui
{
	std::vector < std::shared_ptr<LightSource>> m_lights;
public:
	Gui();
	Gui(GLFWwindow *window);
	~Gui();

	void render();
};
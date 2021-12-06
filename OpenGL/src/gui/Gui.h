#pragma once

#include "vendor/gui/imgui.h"
#include "vendor/gui/imgui_impl_glfw_gl3.h"

#include "lightSource/lightSource.h"
#include "actor/Model.h"

class Gui
{
	std::vector<std::shared_ptr<LightSource>> m_lights;
	std::vector<std::shared_ptr<Model>> m_models;
public:
	Gui();
	Gui(GLFWwindow *window);
	~Gui();


	void addLight(std::shared_ptr<LightSource> light);
	void addModel(std::shared_ptr<Model> model);
	void render();

private:
	void configLight(int i);
	void configModel(int i);
	
};
#include "Gui.h"
#include <iostream>
Gui::Gui()
{
}

Gui::Gui(GLFWwindow *window)
{
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
}

void Gui::render()
{
    // Start the Dear ImGui frame
    ImGui_ImplGlfwGL3_NewFrame();
    ImGui::Begin("Hello");
    //configLight(0);
    configModel(0);
    ImGui::End();


    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

}

// TODO make faster somehow
void Gui::configLight(int i)
{
    float color[3] = { m_lights[i]->m_diffuse.x, m_lights[i]->m_diffuse.y, m_lights[i]->m_diffuse.z };
    ImGui::DragFloat3("color", color, 0.01f, 0.0f, 1.0f);
    m_lights[i]->m_diffuse.x = color[0];
    m_lights[i]->m_diffuse.y = color[1];
    m_lights[i]->m_diffuse.z = color[2];
    /*
    float pos[3] = { m_lights[i]->m_position.x, m_lights[i]->m_position.y, m_lights[i]->m_position.z };
    ImGui::DragFloat3("color", pos, 0.03f, -10.0f, 10.0f);
    m_lights[i]->m_position.x = pos[0];
    m_lights[i]->m_position.y = pos[1];
    m_lights[i]->m_position.z = pos[2];
    */
}

void Gui::configModel(int i)
{
    float pos[3] = { m_models[i]->m_position.x, m_models[i]->m_position.y, m_models[i]->m_position.z };
    ImGui::DragFloat3("position", pos, 0.03f, -10.0f, 10.0f);
    m_models[i]->setPosition(glm::vec3(pos[0], pos[1], pos[2]));

    float scale = m_models[i]->m_scale[0];
    ImGui::DragFloat("scale", &scale, 0.05, 0.001, 100.0f);
    m_models[i]->scale(glm::vec3(scale, scale, scale));

    float rotation = m_models[i]->m_rotation[3];
    ImGui::DragFloat("rotate", &rotation, 0.5f, 0.0f, 360.0f);
    m_models[i]->rotate(glm::vec4(rotation, 1.0f, 1.0f, 1.0f));
}

Gui::~Gui()
{
    //ImGui_ImplGlfwGL3_Shutdown();
    //ImGui::DestroyContext();
}

void Gui::addLight(std::shared_ptr<LightSource> light)
{
    m_lights.push_back(light);
}

void Gui::addModel(std::shared_ptr<Model> model)
{
    m_models.push_back(model);
}

#include "Gui.h"

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

    ImGui::End();
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

}

Gui::~Gui()
{
    //ImGui_ImplGlfwGL3_Shutdown();
    //ImGui::DestroyContext();
}

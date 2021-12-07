#include "gui/Controller.h"
#include "gui/Gui.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>
#include <vector>

Controller controller;  

void mouse_callbackGame(GLFWwindow* window, double xpos, double ypos)
{
    if (controller.m_firstMouse)
    {
        controller.m_lastX = xpos;
        controller.m_lastY = ypos;
        controller.m_firstMouse = false;
    }

    float xoffset = xpos - controller.m_lastX;
    float yoffset = controller.m_lastY - ypos; // reversed since y-coordinates range from bottom to top
    controller.m_lastX = xpos;
    controller.m_lastY = ypos;

    controller.m_camera.rotateCamera(xoffset, yoffset, true);
    controller.m_mousePos = glm::vec2(xpos, ypos);
}

void mouse_callbackGui(GLFWwindow* window, double xpos, double ypos)
{
    controller.m_mousePos = glm::vec2(xpos, ypos);
    controller.m_terrain->setBrushPosition(glm::vec2(controller.m_mousePicker->getIntersectionPoint().x, controller.m_mousePicker->getIntersectionPoint().z));
    //controller.m_models[0]->setPosition(glm::vec3(controller.m_mousePicker->getIntersectionPoint().x,
    //                                              controller.m_mousePicker->getIntersectionPoint().y,
    //                                              controller.m_mousePicker->getIntersectionPoint().z));

}

int main(int argc, char** argv)
{
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

    while (!controller.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        controller.updateGameLogic(mouse_callbackGame, mouse_callbackGui);
    }

    return 0;
}

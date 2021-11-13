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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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
}

int main(int argc, char** argv)
{
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

    while (!controller.shouldClose())
    {
        controller.updateGameLogic(mouse_callback);
    }

    return 0;
}

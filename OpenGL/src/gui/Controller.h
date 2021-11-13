#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>

#include "renderer.h"
#include "gui/Gui.h"

class Controller
{
    Gui m_gui;
    GLFWwindow* m_window;
    Renderer m_renderer;
    ShaderProgram m_mainProgram;
    ShaderProgram m_lightProgram;

    int m_windowWidth;
    int m_windowHeight;
    double m_previousTime;
    double m_currentTime;
    double m_deltaTime;

    std::vector<std::shared_ptr<LightSource>> m_lights;
    std::vector<std::shared_ptr<Model>> m_models;
    bool m_menuOn;

public:
    float m_lastX;
    float m_lastY;
    bool m_firstMouse = true;
    Camera m_camera;

    Controller();
    ~Controller();

    void processInput();
    void mouse_callbackR(GLFWwindow* window, double xpos, double ypos);

    void updateGameLogic(GLFWcursorposfun);

    bool shouldClose();

};


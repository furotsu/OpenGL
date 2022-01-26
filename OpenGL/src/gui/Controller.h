#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <map>

#include "renderer.h"
#include "gui/Gui.h"
#include "glm/glm.hpp"
#include "MousePicker.h"

class Controller
{
public:
    Gui m_gui;
    GLFWwindow* m_window;
    Renderer m_renderer;
    ShaderProgram m_mainProgram;
    ShaderProgram m_lightProgram;
    ShaderProgram m_terrainProgram;
    ShaderProgram m_waterProgram;
    ShaderProgram m_normalProgram;
    ShaderProgram m_skyboxProgram;

    int m_windowWidth;
    int m_windowHeight;
    double m_previousTime;
    double m_currentTime;
    double m_deltaTime;

    std::vector<std::shared_ptr<LightSource>> m_lights;
    std::vector<std::shared_ptr<Model>> m_models;
    std::shared_ptr<Terrain> m_terrain;
    std::shared_ptr<Water> m_water;
    std::shared_ptr<MousePicker> m_mousePicker;
    std::shared_ptr<Skybox> m_skybox;
    bool m_menuOn;

public:
    float m_lastX;
    float m_lastY;
    bool m_firstMouse = true;
    glm::vec2 m_mousePos;
    glm::vec2 m_mouseCameraPos;
    glm::vec4 m_clipPlane;
    Camera m_camera;
    

    Controller();
    ~Controller();

    void initLibrary();

    void processInput();
    bool buttonTriggered(int key);
    void mouse_callbackR(GLFWwindow* window, double xpos, double ypos);

    void updateGameLogic(GLFWcursorposfun, GLFWcursorposfun);

    bool shouldClose();

};


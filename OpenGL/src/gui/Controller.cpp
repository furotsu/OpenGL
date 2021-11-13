#include "Controller.h"

Controller::Controller()
{
    /* Initialize the library */
    if (!glfwInit())
        std::cout << "glfw has not been initialized" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    m_windowWidth = 1600;
    m_windowHeight = 900;
    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Furotsu", NULL, NULL);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!m_window)
    {
        glfwTerminate();
        return ;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "error with glew " << std::endl;
    }
    std::vector<ShaderInfo>  shaders
    {
        { GL_VERTEX_SHADER, "res/shaders/vertex.shader", 0},
        { GL_FRAGMENT_SHADER, "res/shaders/fragment.shader", 0},
    };
    std::vector<ShaderInfo> lightShaders
    {
        { GL_VERTEX_SHADER, "res/shaders/vertexLight.shader", 1},
        { GL_FRAGMENT_SHADER, "res/shaders/fragmentLight.shader", 1},
    };  

    m_models.push_back(std::make_shared<Model>("res/models/boxTextured/BoxTextured.gltf"));
    std::shared_ptr<LightSource> l(std::make_shared<DirectionalLight>(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.2f, 0.2f, 0.2f)));
    m_renderer = Renderer(m_windowWidth, m_windowHeight, 45.0f);
    m_mainProgram = ShaderProgram(shaders);
    m_lightProgram = ShaderProgram(lightShaders);
    m_gui = Gui(m_window);

    m_lights.push_back(l);

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
}

void Controller::processInput()
{
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera.moveCamera(MoveDirection::FORWARD, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera.moveCamera(MoveDirection::BACKWARD, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera.moveCamera(MoveDirection::LEFT, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera.moveCamera(MoveDirection::RIGHT, m_deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE))
    {
        if (m_menuOn)
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            m_menuOn = false;
        }
        else
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_menuOn = true;
        }
    }
}

void Controller::updateGameLogic(GLFWcursorposfun func)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_currentTime = glfwGetTime();
    m_deltaTime = m_currentTime - m_previousTime;
    m_previousTime = m_currentTime;

    if (!m_menuOn)
        glfwSetCursorPosCallback(m_window, func);
    else
        glfwSetCursorPosCallback(m_window, nullptr);

    this->processInput();

    for(int i = 0; i != m_models.size(); ++i)
        m_renderer.draw(m_mainProgram, m_models[i], m_lights, m_camera);

    if(m_menuOn)
        m_gui.render();

    glfwSwapBuffers(m_window);
    glfwPollEvents();

    // Rest of your game loop...
}

Controller::~Controller()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Controller::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}





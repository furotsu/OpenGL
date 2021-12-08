#include "Controller.h"

Controller::Controller()
    : m_mousePos(0, 0)
{
    initLibrary();

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
    std::vector<ShaderInfo> terrainShaders
    {
        { GL_VERTEX_SHADER, "res/shaders/vertexTerrain.shader", 2},
        { GL_FRAGMENT_SHADER, "res/shaders/fragmentTerrain.shader", 2},
        //{ GL_GEOMETRY_SHADER, "res/shaders/geometryTerrain.shader", 2},
    };
    std::vector<ShaderInfo> waterShader
    {
        { GL_VERTEX_SHADER, "res/shaders/vertexWater.shader", 3},
        { GL_FRAGMENT_SHADER, "res/shaders/fragmentWater.shader", 3},
        { GL_GEOMETRY_SHADER, "res/shaders/geometryWater.shader", 3},
    };

    m_terrain = std::make_shared<Terrain>(256, 8, 8, "res/textures/terrain.jpg", "res/textures/heightMap.png");
    m_water = std::make_shared<Water>(256, 4, 8, -2.0f);

    m_models.push_back(std::make_shared<Model>("res/actors/models/duck/Duck.gltf"));
    std::shared_ptr<LightSource> l(std::make_shared<DirectionalLight>(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.2f, 0.2f, 0.2f)));
    m_renderer = Renderer(m_windowWidth, m_windowHeight, 45.0f);

    m_mousePicker = std::make_shared<MousePicker>(m_camera, m_terrain, m_renderer.getProjMat());

    m_mainProgram = ShaderProgram(shaders);
    m_lightProgram = ShaderProgram(lightShaders);
    m_terrainProgram = ShaderProgram(terrainShaders);
    m_waterProgram = ShaderProgram(waterShader);

    m_gui = Gui(m_window);

    m_lights.push_back(l);
    m_gui.addLight(m_lights[0]);
    m_gui.addModel(m_models[0]);
}

void Controller::initLibrary()
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
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "error with glew " << std::endl;
    }

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

    if(buttonTriggered(GLFW_KEY_ESCAPE))
    {
        m_firstMouse = true;
        if (m_menuOn)
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            m_mousePos = m_mouseCameraPos;
            glfwSetCursorPos(m_window, m_mousePos.x, m_mousePos.y);
            m_menuOn = false;
        }
        else
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_mouseCameraPos = m_mousePos;
            m_menuOn = true;
        }
    }
    if (buttonTriggered(GLFW_KEY_P))
    {
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, &polygonMode[0]);
        if (polygonMode[0] == GL_FILL)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (buttonTriggered(GLFW_KEY_U))
    {
        m_terrain->increaseHeight();
    }
    else if (buttonTriggered(GLFW_KEY_J))
    {
        m_terrain->decreaseHeight();
    }
    if (buttonTriggered(GLFW_KEY_0))
    {
        m_terrain->saveHeightMap("testHeightMap.png");
    }
    
}

bool Controller::buttonTriggered(int key)
{
    if (glfwGetKey(m_window, key) == GLFW_PRESS)
    {
        glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
        if (glfwGetKey(m_window, key) == GLFW_RELEASE)
        {
            glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_FALSE);
            return true;
        }
    }
    return false;
}


void Controller::updateGameLogic(GLFWcursorposfun funcGame, GLFWcursorposfun funcGui)
{
    m_currentTime = glfwGetTime();
    m_deltaTime = m_currentTime - m_previousTime;
    m_previousTime = m_currentTime;

    if (!m_menuOn)
        glfwSetCursorPosCallback(m_window, funcGame);
    else
        glfwSetCursorPosCallback(m_window, funcGui);

    this->processInput();

    //glViewport(500, 300, 400, 200);

    m_water->bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    for (int i = 0; i != m_models.size(); ++i)
        m_renderer.draw(m_mainProgram, m_models[i], m_lights, m_camera);

    m_renderer.draw(m_terrainProgram, m_terrain, m_lights, m_camera);
    m_renderer.draw(m_waterProgram, m_water, m_lights, m_camera);

    m_water->unbindFramebuffer();

    for(int i = 0; i != m_models.size(); ++i)
        m_renderer.draw(m_mainProgram, m_models[i], m_lights, m_camera);

    m_renderer.draw(m_terrainProgram, m_terrain, m_lights, m_camera);

    m_renderer.draw(m_waterProgram, m_water, m_lights, m_camera);


    if (m_menuOn)
    {
        m_gui.render();
    }
    m_mousePicker->update(m_camera, m_mousePos);


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





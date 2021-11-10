#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>
#include <vector>

#include "Shader.h"
#include "renderer.h"
#include "shapes.h"
#include "model/model.h"

#include "models/models.h"

Camera camera;

void processInput(GLFWwindow* window, const float &deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveCamera(MoveDirection::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveCamera(MoveDirection::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveCamera(MoveDirection::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveCamera(MoveDirection::RIGHT, deltaTime);
}

float lastX = 400;
float lastY = 400;

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;
    
    camera.rotateCamera(xoffset, yoffset, true);
}

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
	int windowWidth  = 1600;
    int windowHeight = 900;
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Triangles", NULL, NULL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "error with glew " << std::endl;
    }

    // OpenGL stuff here

    Renderer renderer(windowWidth, windowHeight, 45.0f);

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
    
    //Model mainObject("res/models/lantern/lantern.gltf");
    Model mainObject("res/models/lantern/Lantern.gltf");

    ShaderProgram mainProgram(shaders);
    ShaderProgram lightProgram(lightShaders);

    //glScissor(10, 10, 500, 500); // set rectangle from the left bottom corner that will restrict fragments(pixels) from being drawn oustide of it
    //glEnable(GL_SCISSOR_TEST);

    //mainProgram.SetUniform1f("u_width", 800.0f);
    //mainProgram.SetUniform1f("u_height", 800.0f);

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
        
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPointSize(30.0);
    //createCube(1, 2, program);

    double previousTime = glfwGetTime();
    int frameCount = 0;
    
    // 3d stuff goes here
 
    glEnable(GL_DEPTH_TEST);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glm::mat4 m_projMat = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;

        glfwSetCursorPosCallback(window, mouse_callback);

        processInput(window, deltaTime);

        renderer.draw(mainProgram, mainObject, camera);

        //glBindVertexArray(m_VAO);
        //glDrawElements(GL_TRIANGLES, i.size(), GL_UNSIGNED_SHORT, 0);

        //renderer.draw(lightProgram, mainObject, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Rest of your game loop...
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

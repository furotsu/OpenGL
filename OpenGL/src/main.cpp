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
    Model mainObject("res/models/box/Box.gltf");

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
    std::vector<float> v;
    for (int i = 0; i != mainObject.m_meshes[0]->m_vertices.size(); ++i)
    {
        v.push_back(mainObject.m_meshes[0]->m_vertices[i].Position.x);
        v.push_back(mainObject.m_meshes[0]->m_vertices[i].Position.y);
        v.push_back(mainObject.m_meshes[0]->m_vertices[i].Position.z);
    }
    std::vector<Vertex> v2 = mainObject.m_meshes[0]->m_vertices;
    std::vector<unsigned short> i;
    for (int j = 0 ; j != mainObject.m_meshes[0]->m_indices.size(); ++j)
         i.push_back(mainObject.m_meshes[0]->m_indices[j]);
    
    unsigned int m_VAO, m_VBO, m_EBO;
    // 3d stuff goes here
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, v2.size() * sizeof(Vertex), &v2[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(unsigned short), &i[0], GL_STATIC_DRAW);

    //vertices positions
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // TODO - somehow automatize attribIndex setup
    glEnableVertexAttribArray(1);
    //normal values
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    //glEnableVertexAttribArray(1);

    // texture coords
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    //glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    //glEnable(GL_DEPTH_TEST);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glm::mat4 m_projMat = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;

        glfwSetCursorPosCallback(window, mouse_callback);

        processInput(window, deltaTime);

        mainProgram.Bind();
        
        mainProgram.SetUniformMat4f("view", camera.getViewMatrix());
        mainProgram.SetUniformMat4f("projection", m_projMat);
        mainProgram.SetUniformMat4f("model", model);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, i.size(), GL_UNSIGNED_SHORT, 0);

        //renderer.draw(lightProgram, mainObject, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Rest of your game loop...
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>

#include "Shader.h"
#include "shapes.h"
#include "Camera.h"
#include "model.h"

#include "models/models.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void processInput(GLFWwindow* window, const float &deltaTime)
{
        const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

float pitch = 0.0f;
float yaw = -90.0f;

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

    constexpr float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
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
    GLFWwindow *window = glfwCreateWindow(800, 800, "Triangles", NULL, NULL);
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

    GLfloat  vertices[] =
    {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    GLuint indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); // generates 1(first arg) free vertexArrays names into VAO
    glGenBuffers(1, &VBO); // same
    glGenBuffers(1, &EBO); // same
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO); // binds name to newly creates object in memory (OpenGL server)
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // after binding that object becomes the active buffer object, if binding to 0 - stops using buffer obj for that type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL /*vertices*/, GL_STATIC_DRAW); // allocates data of size equal to 2nd argument and copies it from 3rd if not nullptr to last binded object
    void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

    memcpy(data, vertices, sizeof(vertices)); 

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0); // specifies where shader attirbute location can take data from(1st arg)
    glEnableVertexAttribArray(1); // specifies that vertex attrib array assosiated with index can be enabled

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    //glBindVertexArray(0);


   std::vector<ShaderInfo>  shaders
    {
        { GL_VERTEX_SHADER, "res/shaders/vertex.shader", 0},
        { GL_FRAGMENT_SHADER, "res/shaders/fragment.shader", 0},
    };

    ShaderProgram program(shaders);

    glScissor(10, 10, 500, 500); // set rectangle from the left bottom corner that will restrict fragments(pixels) from being drawn oustide of it
    //glEnable(GL_SCISSOR_TEST);
    
    GLint width = glGetUniformLocation(program.m_programID, "u_width");
    GLint height = glGetUniformLocation(program.m_programID, "u_height");
    GLint time = glGetUniformLocation(program.m_programID, "u_time");

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
        
    if (width != -1 && height != -1)
    {
        glUniform1f(width, 800.0f);
        glUniform1f(height, 800.0f);
    }
    else
        std::cout << "cannot send screen size uniforms" << std::endl;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPointSize(30.0);


    double previousTime = glfwGetTime();
    int frameCount = 0;

    // 3d stuff goes here
    
    Camera camera();

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));



    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::rotate(modelMat, glm::radians(45.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    glm::mat4 viewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -6.0f));
    glm::mat4 projMat = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);


    int modelMatID = glGetUniformLocation(program.m_programID, "model");
    int viewMatID  = glGetUniformLocation(program.m_programID, "view");
    int projMatID  = glGetUniformLocation(program.m_programID, "projection");

    glUniformMatrix4fv(modelMatID, 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(viewMatID, 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(projMatID, 1, GL_FALSE, glm::value_ptr(projMat));

    std::cout << modelMatID << ' ' << viewMatID << ' ' << projMatID << std::endl;

    glEnable(GL_DEPTH_TEST);
    //GLuint circleVAO = createCircle(0.2f, 0.2f, 100.0f, program);
    GLuint cubeVAO = createCube(0, 0.0f, program);
    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;

        glfwSetCursorPosCallback(window, mouse_callback);

        glUniform1f(time, (float)glfwGetTime());
        processInput(window, deltaTime);
        glm::mat4 viewMat = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

        glUniformMatrix4fv(viewMatID, 1, GL_FALSE, glm::value_ptr(viewMat));
        glUniformMatrix4fv(projMatID, 1, GL_FALSE, glm::value_ptr(projMat));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, cubePositions[i]);
            float angle = 20.0f * i;
            modelMat = glm::rotate(modelMat, glm::radians(angle + (float)currentTime/20), glm::vec3(1.0f, 0.3f + i/19, 0.5f + i/16));
            glUniformMatrix4fv(modelMatID, 1, GL_FALSE, glm::value_ptr(modelMat));

            drawCube();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        program.Bind();

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Measure FPS
              frameCount++;
        // If a second has passed.
        if (currentTime - previousTime >= 0.0)
        {
            // Display the frame count here any way you want.
            glfwSetWindowTitle(window, std::to_string(frameCount).c_str());

            frameCount = 0;
            previousTime = currentTime;
        }
        // Rest of your game loop...
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>
#include <vector>

#include "Shader.h"
#include "renderer.h"
#include "shapes.h"
#include "Camera.h"
#include "model.h"

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

   float vertices[] = {
       // positions          // normals           // texture coords
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
   };
  
   Model mainObject(vertices, 36, 1, std::vector<std::string>{"res/textures/container2.jpg", "res/textures/container2_specular.jpg"}, GL_TRIANGLES);
   Model lightObject(vertices, 36, 1, GL_TRIANGLES);
    glm::vec3 lightPos(0.1, 2.0f, 6.0f);

    ShaderProgram mainProgram(shaders);
    ShaderProgram lightProgram(lightShaders);

    Renderer renderer;

    //Camera camera;

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

    glm::mat4 modelMat = glm::mat4(1.0f);
    glm::mat4 model2 = glm::mat4(1.0f);
    modelMat = glm::rotate(modelMat, glm::radians(75.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, -4.0f, -2.0f));
    model2 = glm::translate(model2, glm::vec3(0.0f, 2.0f, 0.0f));

    glm::mat4 projMat = glm::perspective(glm::radians(45.0f), (float)windowWidth/(float)windowHeight, 0.1f, 100.0f);

    glm::mat4 lightMat = glm::mat4(1.0f);
    lightMat = glm::translate(lightMat, lightPos);
    lightMat = glm::scale(lightMat, glm::vec3(0.2f));


    glEnable(GL_DEPTH_TEST);
    /*GLuint circleVAO = createCircle(0.2f, 0.2f, 100.0f, program);
    GLuint cubeVAO = createCube(0, 0.0f, program);
    */
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

    glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
    };

    glm::vec3 lightColor(1.0f);
    glm::vec3 ambientColor(1.0f);
    glm::vec3 diffColor(1.0f);

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;

        glfwSetCursorPosCallback(window, mouse_callback);

        //mainProgram.SetUniform1f("u_time", (float)glfwGetTime());
        processInput(window, deltaTime);

        //lightMat = glm::translate(lightMat, glm::vec3(lightPos.x - 1.0f - sin(glfwGetTime()) * 2.0f, lightPos.y - sin(glfwGetTime() / 2.0f)*1.0f, 0.0f));
        //lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        //lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        diffColor = lightColor * glm::vec3(0.5f);
        ambientColor = diffColor * glm::vec3(0.2f);

        mainProgram.Bind();
        //vertex shader uniforms
        mainProgram.SetUniformMat4f("view", camera.getViewMatrix());
        mainProgram.SetUniformMat4f("model", modelMat);
        mainProgram.SetUniformMat4f("projection", projMat);

        //fragment shader uniforms
        mainProgram.SetUniform3f("cameraPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
        mainProgram.SetUniform1i("material.diffuse", 0); // diffuse map (texture itself?)
        mainProgram.SetUniform1i("material.specular", 1);// specular map
        mainProgram.SetUniform1f("material.shininess", 64.0f);
        // directional light
        mainProgram.SetUniform3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
        mainProgram.SetUniform3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        mainProgram.SetUniform3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        mainProgram.SetUniform3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        for (int i = 0; i != 4; ++i)
        {
            std::string number = std::to_string(i);
            mainProgram.SetUniform3f(("pointLights[" + number +  "].position").c_str(), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
            mainProgram.SetUniform3f(("pointLights[" + number + "].ambient").c_str(), 0.05f, 0.05f, 0.05f);
            mainProgram.SetUniform3f(("pointLights[" + number + "].diffuse").c_str(), 0.8f, 0.8f, 0.8f);
            mainProgram.SetUniform3f(("pointLights[" + number + "].specular").c_str(), 1.0f, 1.0f, 1.0f);
            mainProgram.SetUniform1f(("pointLights[" + number + "].constantIntens").c_str(), 1.0f);
            mainProgram.SetUniform1f(("pointLights[" + number + "].linearIntens").c_str(), 0.09);
            mainProgram.SetUniform1f(("pointLights[" + number + "].quadraticIntens").c_str(), 0.032);
        }

        // spotLight    
        mainProgram.SetUniform3f("flashLight.position", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
        mainProgram.SetUniform3f("flashLight.direction", camera.getFront().x, camera.getFront().y, camera.getFront().z);
        mainProgram.SetUniform3f("flashLight.ambient", 0.0f, 0.0f, 0.0f);
        mainProgram.SetUniform3f("flashLight.diffuse", 1.0f, 1.0f, 1.0f);
        mainProgram.SetUniform3f("flashLight.specular", 1.0f, 1.0f, 1.0f);
        mainProgram.SetUniform1f("flashLight.constantIntens", 1.0f);
        mainProgram.SetUniform1f("flashLight.linearIntens", 0.09);
        mainProgram.SetUniform1f("flashLight.quadraticIntens", 0.032);
        mainProgram.SetUniform1f("flashLight.innerCutOff", glm::cos(glm::radians(12.5f)));
        mainProgram.SetUniform1f("flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        renderer.draw(mainProgram, mainObject);
        for (unsigned int i = 0; i != 10; i++)
        {
            glm::mat4 modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, cubePositions[i]);
            float angle = 20.0f * i;
            modelMat = glm::rotate(modelMat, glm::radians(angle + (float)currentTime/20), glm::vec3(1.0f, 0.3f + i/19, 0.5f + i/16));
            mainProgram.SetUniformMat4f("model", modelMat);

            renderer.draw(mainProgram, mainObject);
        }

        lightProgram.Bind();
        lightProgram.SetUniformMat4f("view", camera.getViewMatrix());
        lightProgram.SetUniformMat4f("model", lightMat);
        lightProgram.SetUniformMat4f("projection", projMat);
        lightProgram.SetUniform3f("lightColor", diffColor.x, diffColor.y, diffColor.z);
        
        renderer.draw(lightProgram, lightObject);
        for (unsigned int i = 0; i != 4; i++)
        {
            glm::mat4 lightMat = glm::mat4(1.0f);
            lightMat = glm::translate(lightMat, pointLightPositions[i]);
            lightMat = glm::scale(lightMat, glm::vec3(0.1f, 0.1f, 0.1f));

            lightProgram.SetUniformMat4f("model", lightMat);

            glDrawArrays(GL_TRIANGLES, 0, 36);
            renderer.draw(lightProgram, lightObject);
        }
        
        //program.Bind(); 

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

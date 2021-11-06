#include "renderer.h"

#include <iostream>
Renderer::Renderer(int windowWidth, int windowHeight, float fovDegrees)
	: m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
	m_projMat = glm::perspective(glm::radians(fovDegrees), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
}

void Renderer::draw(ShaderProgram& program, Model& model, Camera &camera) 
{
	program.Bind();

	//model related uniforms

	//camera related uniforms
	program.SetUniformMat4f("view", camera.getViewMatrix());
	program.SetUniformMat4f("projection", m_projMat);

    //fragment shader uniforms
    program.SetUniform3f("cameraPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    //program.SetUniform1i("material.diffuse", 0); // diffuse map (texture itself?)
    //program.SetUniform1i("material.specular", 1);// specular map
    //program.SetUniform1f("material.shininess", 64.0f);
    // directional light
    program.SetUniform3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
    program.SetUniform3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    program.SetUniform3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    program.SetUniform3f("dirLight.specular", 0.5f, 0.5f, 0.5f);

    // point light 1
    /*for (int i = 0; i != 4; ++i)
    {
    std::string number = std::to_string(i);
    program.SetUniform3f(("pointLights[" + number +  "].position").c_str(), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
    program.SetUniform3f(("pointLights[" + number + "].ambient").c_str(), 0.05f, 0.05f, 0.05f);
    program.SetUniform3f(("pointLights[" + number + "].diffuse").c_str(), 0.8f, 0.8f, 0.8f);
    program.SetUniform3f(("pointLights[" + number + "].specular").c_str(), 1.0f, 1.0f, 1.0f);
    program.SetUniform1f(("pointLights[" + number + "].constantIntens").c_str(), 1.0f);
    program.SetUniform1f(("pointLights[" + number + "].linearIntens").c_str(), 0.09);
    program.SetUniform1f(("pointLights[" + number + "].quadraticIntens").c_str(), 0.032);
    }*/

    // spotLight    
    program.SetUniform3f("flashLight.position", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    program.SetUniform3f("flashLight.direction", camera.getFront().x, camera.getFront().y, camera.getFront().z);
    program.SetUniform3f("flashLight.ambient", 0.0f, 0.0f, 0.0f);
    program.SetUniform3f("flashLight.diffuse", 1.0f, 1.0f, 1.0f);
    program.SetUniform3f("flashLight.specular", 1.0f, 1.0f, 1.0f);
    program.SetUniform1f("flashLight.constantIntens", 1.0f);
    program.SetUniform1f("flashLight.linearIntens", 0.09);
    program.SetUniform1f("flashLight.quadraticIntens", 0.032);
    program.SetUniform1f("flashLight.innerCutOff", glm::cos(glm::radians(12.5f)));
    program.SetUniform1f("flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    model.draw(program);
}
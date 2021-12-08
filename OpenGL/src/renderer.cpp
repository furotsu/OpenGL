#include "renderer.h"

#include <iostream>

Renderer::Renderer(int windowWidth, int windowHeight, float fovDegrees)
	: m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
	m_projMat = glm::perspective(glm::radians(fovDegrees), (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f);
}

void Renderer::draw(ShaderProgram& program, std::shared_ptr<Model> model, std::vector<std::shared_ptr<LightSource>> &lightSources, Camera &camera)
{
    glCullFace(GL_BACK);
	program.Bind();

	//model related uniforms

	//camera related uniforms
	program.SetUniformMat4f("view", camera.getViewMatrix());
	program.SetUniformMat4f("projection", m_projMat);

    //fragment shader uniforms
    program.SetUniform3f("cameraPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

    for (int i = 0; i != lightSources.size(); ++i)
    {
        lightSources[i]->bindUniforms(program);
    }

    model->draw(program);
}

void Renderer::draw(ShaderProgram& program, std::shared_ptr<Terrain> terrain, std::vector<std::shared_ptr<LightSource>>& lightSources, Camera& camera)
{
    glCullFace(GL_FRONT);
    program.Bind();

    //model related uniforms
    program.SetUniform1f("u_time", glfwGetTime());

    //camera related uniforms
    program.SetUniformMat4f("view", camera.getViewMatrix());
    program.SetUniformMat4f("projection", m_projMat);
    program.SetUniform3f("cameraPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);


    //fragment shader uniforms
    program.SetUniform3f("cameraPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

    for (int i = 0; i != lightSources.size(); ++i)
    {
        lightSources[i]->bindUniforms(program);
    }

    terrain->draw(program);
}

void Renderer::draw(ShaderProgram& program, std::shared_ptr<Water> terrain, std::vector<std::shared_ptr<LightSource>>& lightSources, Camera& camera)
{
    glDisable(GL_CULL_FACE);

    program.Bind();

    //terrain->bindFramebuffer();
    glBindTexture(GL_TEXTURE_2D, terrain->m_WFB->m_reflectionTexture);

    //model related uniforms
    program.SetUniform1f("u_time", glfwGetTime());

    //camera related uniforms
    program.SetUniformMat4f("view", camera.getViewMatrix());
    program.SetUniformMat4f("projection", m_projMat);
    program.SetUniform3f("cameraPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);


    //fragment shader uniforms
    program.SetUniform3f("cameraPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

    for (int i = 0; i != lightSources.size(); ++i)
    {
        lightSources[i]->bindUniforms(program);
    }

    terrain->draw(program);

    glEnable(GL_CULL_FACE);
}

glm::mat4 Renderer::getProjMat()
{
    return m_projMat;
}
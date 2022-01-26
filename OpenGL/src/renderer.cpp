#include "renderer.h"

#include <iostream>

Renderer::Renderer(int windowWidth, int windowHeight, float fovDegrees)
	: m_windowWidth(windowWidth), m_windowHeight(windowHeight), m_drawNormals(false)
{
	m_projMat = glm::perspective(glm::radians(fovDegrees), (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f);
}

void Renderer::draw(ShaderProgram& program, std::shared_ptr<Model> model, std::vector<std::shared_ptr<LightSource>>& lightSources, Camera& camera, glm::vec4 clipPlane)
{
    glCullFace(GL_BACK);
	program.Bind();

    program.SetUniform4f("clipPlane", clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);

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

void Renderer::draw(ShaderProgram& program, std::shared_ptr<Terrain> terrain, std::vector<std::shared_ptr<LightSource>>& lightSources, Camera& camera, glm::vec4 clipPlane)
{
    glCullFace(GL_FRONT);
    program.Bind();

    program.SetUniform4f("clipPlane", clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);


    //model related uniforms
    program.SetUniform1f("u_time", glfwGetTime());

    //camera related uniforms
    program.SetUniformMat4f("view", camera.getViewMatrix());
    program.SetUniformMat4f("projection", m_projMat);
    //program.SetUniformMat4f("model")

    program.SetUniform3f("cameraPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);


    //fragment shader uniforms
    program.SetUniform3f("cameraPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

    for (int i = 0; i != lightSources.size(); ++i)
    {
        lightSources[i]->bindUniforms(program);
    }

    terrain->draw(program);
}

void Renderer::draw(ShaderProgram& program, std::shared_ptr<Water> water, std::vector<std::shared_ptr<LightSource>>& lightSources, Camera& camera, std::shared_ptr<ShaderProgram> normalProgram)
{
    glDisable(GL_CULL_FACE);

    program.Bind();

    //terrain->bindFramebuffer();
    
    //TODO change WFB texture to texture object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, water->m_WFB->m_reflectionTexture);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, water->m_WFB->m_refractionTexture);

    double time = glfwGetTime();

    //model related uniforms
    program.SetUniform1f("u_time", time);

    //camera related uniforms
    program.SetUniformMat4f("view", camera.getViewMatrix());
    program.SetUniformMat4f("projection", m_projMat);
    glm::mat4 modelMat = water->getModelMat();
    program.SetUniformMat4f("model", modelMat);

    program.SetUniform3f("cameraPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);


    //fragment shader uniforms

    for (int i = 0; i != lightSources.size(); ++i)
    {
        lightSources[i]->bindUniforms(program);
    }

    water->draw(program);

    if (m_drawNormals && normalProgram != nullptr)
    {
        normalProgram->Bind();

        normalProgram->SetUniform1f("u_time", time);

        normalProgram->SetUniformMat4f("view", camera.getViewMatrix());
        normalProgram->SetUniformMat4f("projection", m_projMat);
        glm::mat4 modelMat = water->getModelMat();
        //normalProgram->SetUniformMat4f("model", modelMat);

        program.SetUniform3f("cameraPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

        water->drawNormals(normalProgram);
    }


    glEnable(GL_CULL_FACE);
}

void Renderer::draw(ShaderProgram& program, std::shared_ptr<Skybox> skybox, Camera& camera, glm::vec4 clipPlane)
{
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    program.Bind();

    program.SetUniform4f("clipPlane", clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);

    //removing the translation component of view matrix so skybox will be always around the player
    glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));


    //camera related unifroms
    program.SetUniformMat4f("view", view);
    program.SetUniformMat4f("projection", m_projMat);

    skybox->draw(program);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
}

glm::mat4 Renderer::getProjMat()
{
    return m_projMat;
}
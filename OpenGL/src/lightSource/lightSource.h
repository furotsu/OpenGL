#pragma once

#include "vendor/glm/glm.hpp"

#include "Shader.h"

class LightSource
{
protected:
	glm::vec3 m_direction;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;

	glm::vec3 m_position;
	glm::vec3 m_specular;

	float m_constantIntens;
	float m_linearIntens;
	float m_quadraticIntens;

	float m_innerCutOff;
	float m_outerCutOff;

	static int s_dirLCount;
	static int s_pointLCount;
	static int s_flashLCount;

public:
	virtual void bindUniforms(ShaderProgram& program);

	friend class Gui;
};

class DirectionalLight : public LightSource
{
private:
	static int s_dirPos;
public:
	DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse);
	DirectionalLight(DirectionalLight&) = default;
	~DirectionalLight();

	void bindUniforms(ShaderProgram& program) override;
};

class PointLight : public LightSource
{
private:
	static int s_pointPos;
public:
	PointLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,float intensity);
	PointLight(PointLight&) = default;
	~PointLight();

	void bindUniforms(ShaderProgram& program) override;

};

class FlashLight : public LightSource
{
private:
	static int s_flashPos;
public:
	FlashLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float CutOff, float intensity);
	FlashLight(FlashLight&) = default;
	~FlashLight();

	void bindUniforms(ShaderProgram& program) override;
};

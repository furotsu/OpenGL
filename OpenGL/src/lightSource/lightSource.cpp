#include "lightSource.h"

int LightSource::s_dirLCount = 0;
int LightSource::s_pointLCount = 0;
int LightSource::s_flashLCount = 0;

int DirectionalLight::s_dirPos = 0;
int PointLight::s_pointPos = 0;
int FlashLight::s_flashPos = 0;

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse)
{
	this->s_dirLCount++;
	m_direction = direction;
	m_ambient = ambient;
	m_diffuse = diffuse;
}

DirectionalLight::~DirectionalLight()
{
	this->s_dirLCount--;
}

void DirectionalLight::bindUniforms(ShaderProgram& program)
{
	program.SetUniform3f("dirLight.direction", m_direction.x, m_direction.y, m_direction.z);
	program.SetUniform3f("dirLight.ambient", m_ambient.x, m_ambient.y, m_ambient.z);
	program.SetUniform3f("dirLight.diffuse", m_diffuse.x, m_diffuse.y, m_diffuse.z);
	//program.SetUniform3f("dirLight.specular", );
}

PointLight::PointLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float intensity)
{
	this->s_pointLCount++;
	m_direction = direction;
	m_ambient = ambient;
	m_diffuse = diffuse;

	m_position = position;
	m_specular = specular;
	
	m_constantIntens = intensity;
	m_linearIntens = intensity;
	m_quadraticIntens = intensity;

}

PointLight::~PointLight()
{
	this->s_pointLCount--;
}

void PointLight::bindUniforms(ShaderProgram& program)
{
	if (this->s_pointPos == 0)
		program.SetUniform1i("pointLightsCount", this->s_pointLCount);

	std::string number = std::to_string(s_pointPos);
	program.SetUniform3f(("pointLights[" + number + "].position").c_str(), m_position.x, m_position.y, m_position.z);
	program.SetUniform3f(("pointLights[" + number + "].ambient").c_str(), m_ambient.x, m_ambient.y, m_ambient.z);
	program.SetUniform3f(("pointLights[" + number + "].diffuse").c_str(), m_diffuse.x, m_diffuse.y, m_diffuse.z);
	program.SetUniform3f(("pointLights[" + number + "].specular").c_str(), m_specular.x, m_specular.y, m_specular.z);
	program.SetUniform1f(("pointLights[" + number + "].constantIntens").c_str(), m_constantIntens);
	program.SetUniform1f(("pointLights[" + number + "].linearIntens").c_str(), m_linearIntens);
	program.SetUniform1f(("pointLights[" + number + "].quadraticIntens").c_str(), m_quadraticIntens);

	this->s_pointPos++;
	if (s_pointPos == s_pointLCount)
		s_pointPos = 0;
}

FlashLight::FlashLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float CutOff, float intensity)
{
	this->s_flashLCount++;

	m_direction = direction;
	m_ambient = ambient;
	m_diffuse = diffuse;

	m_position = position;
	m_specular = specular;

	m_constantIntens = intensity;
	m_linearIntens = intensity;
	m_quadraticIntens = intensity;

	m_innerCutOff = CutOff;
	m_outerCutOff = CutOff + 1.0f;
}

FlashLight::~FlashLight()
{
	this->s_flashLCount--;
}

void FlashLight::bindUniforms(ShaderProgram& program)
{
	if (s_flashPos == 0)
		program.SetUniform1i("flashLightsCount", this->s_flashLCount);

	std::string number = std::to_string(s_flashPos);
	program.SetUniform3f(("flashLights[" + number + "].position").c_str(), m_position.x, m_position.y, m_position.z);
	program.SetUniform3f(("flashLights[" + number + "].ambient").c_str(), m_ambient.x, m_ambient.y, m_ambient.z);
	program.SetUniform3f(("flashLights[" + number + "].diffuse").c_str(), m_diffuse.x, m_diffuse.y, m_diffuse.z);
	program.SetUniform3f(("flashLights[" + number + "].specular").c_str(), m_specular.x, m_specular.y, m_specular.z);
	program.SetUniform1f(("flashLights[" + number + "].constantIntens").c_str(), m_constantIntens);
	program.SetUniform1f(("flashLights[" + number + "].linearIntens").c_str(), m_linearIntens);
	program.SetUniform1f(("flashLights[" + number + "].quadraticIntens").c_str(), m_quadraticIntens);
	program.SetUniform1f(("flashLights[" + number + "].innerCutOff").c_str(), glm::cos(glm::radians(m_innerCutOff)));
	program.SetUniform1f(("flashLights[" + number + "].outerCutOff").c_str(), glm::cos(glm::radians(m_outerCutOff)));

	this->s_flashPos++;
	if (this->s_flashLCount == s_flashLCount)
		s_flashPos = 0;	
}

void LightSource::bindUniforms(ShaderProgram& program)
{
}

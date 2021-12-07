#pragma once

#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Camera.h"
#include "terrain/Terrain.h"
#include "debugger.h"

constexpr int PRECISION  = 200;
constexpr float RAY_LENGTH = 20.0f;

class MousePicker
{
private:
	glm::vec3 m_ray;
	glm::vec3 m_camPos;
	glm::vec2 m_mousePos;

	glm::mat4 m_projectionM;
	glm::mat4 m_viewM;
	glm::vec3 m_intersectionPoint;

	std::shared_ptr<Terrain> m_terrain;

public:
	MousePicker(Camera& camera, std::shared_ptr<Terrain> terrain, glm::mat4 projection);

	void update(Camera&, glm::vec2);
	glm::vec3 getIntersectionPoint();
private:
	glm::vec3 calculateRay(Camera&);

	glm::vec2 normDevCoord(float, float);
	glm::vec4 eyesCoord(glm::vec4);
	glm::vec3 worldCoords(glm::vec4);

	glm::vec3 getPointOnRay(glm::vec3 ray, float distance);
	glm::vec3 binarySearch(float start, float finish, glm::vec3 ray);
	bool intersectionInRange(float start, float finish, glm::vec3 ray);
	bool isUnderGround(glm::vec3 testPoint);


};

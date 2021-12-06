#include "MousePicker.h"
#include <iostream>


MousePicker::MousePicker(Camera& camera, std::shared_ptr<Terrain> terrain, glm::mat4 projection)
{
	m_projectionM = projection;
	m_viewM = camera.getViewMatrix();
	m_terrain = terrain;
}

//TODO add global const
void MousePicker::update(Camera& camera, glm::vec2 mousePos)
{
	m_viewM = camera.getViewMatrix();
	m_camPos = camera.getPosition();
	m_mousePos = mousePos;
	glm::vec3 ray = calculateRay(camera);
	if (intersectionInRange(0, RAY_LENGTH, ray)) 
	{
		m_intersectionPoint = binarySearch(0, RAY_LENGTH, ray);
	}
	else 
	{
		m_intersectionPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

glm::vec3 MousePicker::getIntersectionPoint()
{
	return m_intersectionPoint;
}

glm::vec3 MousePicker::calculateRay(Camera& camera)
{
	glm::vec2 rayNormCoords = normDevCoord(m_mousePos.x, m_mousePos.y);
	glm::vec4 rayClipCoords = glm::vec4(rayNormCoords, -1.0f, 1.0f);
	glm::vec4 rayEyeCoords = eyesCoord(rayClipCoords);
	glm::vec3 rayWorldCoords = worldCoords(rayEyeCoords);
	return rayWorldCoords;
}

glm::vec2 MousePicker::normDevCoord(float posX, float posY)
{
	float x = (2.0f * posX) / 1600.0f - 1.0f;
	float y = 1.0f - (2.0f * posY) / 900.0f;

	return glm::vec2(x, y);
}

glm::vec4 MousePicker::eyesCoord(glm::vec4 clipCoords)
{
	glm::mat4 invProj = glm::inverse(m_projectionM);
	glm::vec4 res = invProj * clipCoords;
	return glm::vec4(res.x, res.y, -1.0f, 0.0f);
}

glm::vec3 MousePicker::worldCoords(glm::vec4 eyeCoords)
{
	glm::mat4 invView = glm::inverse(m_viewM);
	glm::vec4 tmp = invView * eyeCoords;
	glm::vec3 res = glm::normalize(glm::vec3(tmp.x, tmp.y, tmp.z));

	return res;
}

glm::vec3 MousePicker::getPointOnRay(glm::vec3 ray, float distance) 
{
	glm::vec3 start = glm::vec3(m_camPos.x, m_camPos.y, m_camPos.z);
	glm::vec3 scaledRay = glm::vec3(ray.x * distance, ray.y * distance, ray.z * distance);
	return start + scaledRay;
}

glm::vec3 MousePicker::binarySearch(float start, float finish, glm::vec3 ray) 
{
	float half = start + ((finish - start) / 2.0f);
	float currPresicion = finish - start;
	glm::vec3 res = ray;
	int i = 0;
	while (i != 50)
	{
		if (intersectionInRange(start, half, ray)) 
		{
			finish = half;
			half = start + ((half - start) / 2.0f);
		}
		else 
		{
			start = half;
			half = half + ((finish - half) / 2.0f);
		}
		currPresicion /= 2;
		i++;
	}
	return getPointOnRay(ray, half);
}

bool MousePicker::intersectionInRange(float start, float finish, glm::vec3 ray) 
{
	glm::vec3 startPoint = getPointOnRay(ray, start);
	glm::vec3 endPoint = getPointOnRay(ray, finish);
	if (isUnderGround(endPoint))
	{
		//std::cout << "lol";
		return true;
	}
	else
	{
		return false;
	}
}

bool MousePicker::isUnderGround(glm::vec3 testPoint)
{
	float height = 0;

	height = m_terrain->getHeight(testPoint.x, testPoint.z);
	
	if (height != INVALID_HEIGHT && testPoint.y < height)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

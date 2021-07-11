#include "Camera.h"


Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraUp, const float& yaw, const float& pitch)
	: m_position(cameraPos), m_worldUp(cameraUp), m_yaw(yaw), m_pitch(pitch), m_speed(SPEED), m_sensitivity(SENSITIVITY)
{
	updateVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: m_position(glm::vec3(posX, posY, posZ)), m_worldUp(glm::vec3(upX, upY, upZ)), m_yaw(yaw), m_pitch(pitch), m_speed(SPEED), m_sensitivity(SENSITIVITY)
{
	updateVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return m_viewMatrix;
}

void Camera::moveCamera(MoveDirection direction, float deltaTime)
{
	if (direction == MoveDirection::FORWARD)
	 m_position += m_speed * deltaTime * m_front;
	if (direction == MoveDirection::BACKWARD)
	 m_position -= m_speed * deltaTime * m_front;
	if (direction == MoveDirection::LEFT)
	 m_position -= m_speed * deltaTime * m_right;
	if (direction == MoveDirection::RIGHT)
	 m_position += m_speed * deltaTime * m_right;
}

void Camera::rotateCamera(float xoffset, float yoffset, GLboolean constrainPitch = true)
{
	xoffset *= m_sensitivity;
	yoffset *= m_sensitivity;

	m_yaw   += xoffset;
	m_pitch += yoffset;

	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	updateVectors();
}
void Camera::updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
	m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

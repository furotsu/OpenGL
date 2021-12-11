#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

// TODO lol
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 1.0f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

enum class MoveDirection
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;
	glm::mat4 m_viewMatrix;

	float m_speed;
	float m_sensitivity;
	float m_yaw;
	float m_pitch;
public:
	Camera(glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f), const float& yaw = YAW, const float& pitch = PITCH);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = YAW, float pitch = PITCH);

	glm::mat4& getViewMatrix();
	glm::vec3 getPosition() const;
	glm::vec3 getFront() const;

	void setPosition(glm::vec3);
	void setYawPitch(float, float);

	void moveCamera(MoveDirection direction, float deltaTime);
	void rotateCamera(float, float, GLboolean);

	void mirrorHor(float);

private:
	void updateVectors();
};
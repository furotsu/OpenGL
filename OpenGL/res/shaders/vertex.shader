#version 430 core

layout(location = 1) in vec3 vPosition;
layout(location = 2) in vec3 normal;

uniform float u_time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0);

	Normal = mat3(transpose(inverse(model))) * normal;
	FragPos = vec3(model * vec4(vPosition, 1.0f));
}
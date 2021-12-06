#version 430 core

#define PI 3.1415926538

layout(location = 1) in vec3 vPosition;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;
uniform float u_time;

out DATA
{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 Normal; // delete this?
	vec3 FragColor;
	mat4 projection;
	float height;
} data_out;

void main()
{
	data_out.Normal = normal;
	data_out.FragPos = vPosition;
	data_out.TexCoords = texCoord * 40.0f;
	data_out.projection = projection;
	vec3 resPos = vPosition;
	float k = 2 * PI / 1.0f;
	float f = k * (vPosition.x - 1.0f * u_time);
	resPos.x += cos(0.5f * resPos.x + 3.0f * u_time) / 8.0f;
	resPos.y = sin(2.0f * resPos.x + 2.0 * u_time) / 10.0f;
	data_out.height = resPos.y;

	gl_Position = view * vec4(resPos, 1.0f);
	data_out.FragColor = vec3(0.1f, 0.1f, 0.8f);
}
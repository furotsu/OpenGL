#version 430 core

#define PI 3.1415926538

layout(location = 1) in vec3 vPosition;
layout(location = 2) in vec3 vColor;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform float u_time;

uniform float amplitude;


out DATA
{
	vec3 FragPos;
	vec3 FragColor;
	mat4 projection;
	vec2 textureCoords;
} data_out;

void main()
{
	data_out.FragPos = vPosition;
	data_out.projection = projection;
	data_out.FragColor = normalize(vColor);


	vec3 resPos = vPosition;
	float k = 2 * PI / 1.0f;
	float f = k * (vPosition.x - 1.0f * u_time);
	resPos.x += cos(0.5f * resPos.x + 3.0f * u_time) / 8.0f;
	resPos.y = 0.02f * sin(2.0f * resPos.x + 1.0 * u_time) + vPosition.y;
	//resPos.y = sin(2.0f * resPos.x + speed * u_time) / amplitude + vPosition.y;

	data_out.textureCoords = vec2((-resPos.x + vPosition.x)/8.0f, resPos.y );

	//gl_Position = view * model * vec4(resPos, 1.0f);
	gl_Position = view * vec4(resPos, 1.0f);
}
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
	vec3 normal;
} data_out;

void main()
{
	data_out.FragPos = vPosition;
	data_out.projection = projection;
	data_out.FragColor = normalize(vColor);


	vec3 resPos = vPosition;

	float amplitude = 0.03f;

	float k = 2.0f;
	float f = k * (2.0f * resPos.x + 1.0 * u_time);

	resPos.x += amplitude * cos(f);
	resPos.y = amplitude * sin(f) + vPosition.y;


	vec3 tangent = normalize(vec3(
		1 - k * amplitude * sin(f),
		k * amplitude * cos(f),
		0
		));
	vec3 normal = vec3(-tangent.y, tangent.x, 0);
	data_out.normal = normal;

	//resPos.y = sin(2.0f * resPos.x + speed * u_time) / amplitude + vPosition.y;

	data_out.textureCoords = vec2((-resPos.x + vPosition.x)/8.0f, resPos.y );

	//gl_Position = view * model * vec4(resPos, 1.0f);
	gl_Position = view * vec4(resPos, 1.0f);
}
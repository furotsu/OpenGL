#version 430 core

#define PI 3.1415926538

layout(location = 1) in vec3 vPosition;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;
uniform vec2 circlePosition;
uniform float brushSize;
uniform float u_time;
uniform float isChanging;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 vColor;

void main()
{
	Normal = normal;
	FragPos = vPosition;
	TexCoords = texCoord;// *40.0f;
	vColor = vec3(0.0f, 0.0f, 0.0f);

	if (isChanging != 0.0f)
	{
		if (abs(distance(vPosition.xz, circlePosition)) < brushSize)
		{
			float color = smoothstep(0.6f, 0.01f, distance(vPosition.xz, circlePosition));
			vColor = vec3(color, color, color);
		}
	}

	gl_Position = projection * view * vec4(vPosition, 1.0f);
}
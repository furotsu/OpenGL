#version 430 core

layout(location = 1) in vPosition;
layout(location = 2) in normal;
layout(location = 3) in texCoord;

uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	Normal = normal;
	FragPos = vPosition;
	TexCoords = texCoord;

	gl_position = projection * view * vec4(vPosition, 1.0f);
}
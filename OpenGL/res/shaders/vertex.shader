#version 430 core

layout(location = 1) in vec3 vPosition;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoords;

uniform float u_time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 clipPlane;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()	
{

	Normal = mat3(transpose(inverse(model))) * normal;
	FragPos = vec3(model * vec4(vPosition, 1.0f));
	TexCoords = texCoords;

	vec4 worldPosition =  model * vec4(vPosition, 1.0f);

	gl_ClipDistance[0] = dot(worldPosition, clipPlane);

	gl_Position = projection * view * worldPosition;
}
#version 430 core

layout(location = 0) in vec3 vPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 clipPlane;


void main()
{
	vec4 worldPosition = model * vec4(vPosition, 1.0f);

	gl_ClipDistance[0] = dot(worldPosition, clipPlane);

	gl_Position = projection * view * worldPosition;
}
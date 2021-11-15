#version 430 core

out vec4 fColor;

uniform sampler2D texture_diffuse;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
	fColor = vec4(vec3(texture(texture_diffuse, TexCoords)), 1.0f);
}
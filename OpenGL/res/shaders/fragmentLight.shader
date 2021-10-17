#version 430 core

out vec4 fColor;

uniform vec3 lightColor;

void main()
{
	vec3 result = lightColor;
	result.x = max(min(lightColor.x * 1.5f, 0.9f), lightColor.x + 0.5f);
	result.y = max(min(lightColor.y * 1.5f, 0.9f), lightColor.y + 0.5f);
	result.z = max(min(lightColor.z * 1.5f, 0.9f), lightColor.z + 0.5f);
	fColor = vec4(result, 1.0f);
}
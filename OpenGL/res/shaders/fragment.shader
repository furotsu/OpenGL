#version 430 core

out vec4 fColor;

uniform float u_width;
uniform float u_height;
uniform float u_time;

void main()
{
	float x = gl_FragCoord.x / 800;
	float y = gl_FragCoord.y / 800;
	fColor = vec4(x, y, abs(sin(u_time)), 1.0);
}

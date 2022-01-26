#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 2) out;
//layout(triangle_strip, max_vertices = 3) out;

const float MAGNITUDE = 0.1;

in DATA
{
    vec3 FragPos;
    vec3 FragColor;
    mat4 projection;
    vec2 textureCoords;
} data_in[];

vec3 getNormal()
{
    vec3 x = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
    vec3 y = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
    return normalize(cross(x, y));
}

uniform mat4 projection;

void main()
{
    gl_Position = projection * gl_in[0].gl_Position;
    EmitVertex();
    //gl_Position = projection * (gl_in[index].gl_Position + vec4(getNormal(), 0.0) * MAGNITUDE);
    //gl_Position = projection * (gl_in[0].gl_Position + vec4(0.0, 1.0, 0.0, 0.0) * MAGNITUDE);
   // EmitVertex();
    gl_Position = projection * (gl_in[0].gl_Position + vec4(getNormal(), 0.0f) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}
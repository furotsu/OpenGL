#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 FragColor;
out float height;

in DATA
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
    vec3 FragColor;
    mat4 projection;
    float height;
} data_in[];

vec3 getNormal()
{
    vec3 x = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
    vec3 y = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
    return normalize(cross(x, y));
}

void main()
{

    for (int i = 0; i != 3; ++i)
    {
        gl_Position = data_in[i].projection * gl_in[i].gl_Position;
        FragPos = data_in[i].FragPos;
        Normal = getNormal();
        TexCoords = data_in[i].TexCoords;
        FragColor = data_in[i].FragColor;
        height = data_in[i].height;

        EmitVertex();
    }
    EndPrimitive();
}
#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


out vec3 FragPos;
out vec3 Normal;
out vec3 FragColor;
out vec4 clipSpace;
out vec2 textureCoords;
    
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

void main()
{

    for (int i = 0; i != 3; ++i)
    {
        clipSpace = data_in[i].projection * gl_in[i].gl_Position;
        gl_Position = clipSpace;
        FragPos = data_in[i].FragPos;
        Normal = getNormal();
        FragColor = data_in[i].FragColor;
        textureCoords = data_in[i].textureCoords;

        EmitVertex();
    }
    EndPrimitive();
}
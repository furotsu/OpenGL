#version 430 core 

layout(location = 1) in vec3 vPosition;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

uniform vec4 clipPlane;

void main()
{
    gl_ClipDistance[0] = dot(vec4(vPosition, 1.0f), clipPlane);

    TexCoords = vPosition;
    vec4 pos = projection * view * vec4(vPosition, 1.0f);
    gl_Position = pos.xyww;
}
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aTint;

out vec2 TexCoord;
out vec3 Tint;

layout(std140) uniform Matrices {
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main()
{
    TexCoord = aTexCoord;
    Tint = aTint;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
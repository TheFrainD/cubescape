#version 330 core

in vec2 TexCoord;
in vec3 Tint;

uniform sampler2D texture1;

out vec4 FragColor;

void main()
{
    FragColor = texture(texture1, TexCoord) * vec4(Tint, 1.0);
}
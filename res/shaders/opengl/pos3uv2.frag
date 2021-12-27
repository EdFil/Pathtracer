#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D Texture01;

void main()
{
    FragColor = texture(Texture01, TexCoords) + vec4(1.0, 0.0, 0.0, 1.0);
}

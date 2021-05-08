#version 330 core

in vec3 Color;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D Texture01;

void main()
{
    FragColor = vec4(Color, 1.0) * texture(Texture01, TexCoords);
}

#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormals;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
} matrices;

layout (std140) uniform Light {
    vec3 position;
    vec3 diffuseColor;
} light;

uniform mat4 modelMatrix;

out vec3 Color;
out vec2 TexCoords;

void main()
{
    Color = aNormals;
    TexCoords = aTexCoords;
    gl_Position =  matrices.projection * matrices.view * modelMatrix * vec4(aPosition, 1.0);
}

#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec4 cameraPosition;
} matrices;

uniform mat4 modelMatrix;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = matrices.projection * matrices.view * modelMatrix * vec4(aPosition, 1.0f);
}

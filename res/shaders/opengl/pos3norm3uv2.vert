#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormals;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
};
uniform mat4 modelMatrix;

out vec3 Color;
out vec2 TexCoords;

void main()
{
    Color = vec3(abs(aNormals.x), abs(aNormals.y), abs(aNormals.z));
    TexCoords = aTexCoords;
    gl_Position =  projection * view * modelMatrix * vec4(aPosition, 1.0);
}

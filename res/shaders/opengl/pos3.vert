#version 330 core

layout (location = 0) in vec3 aPosition;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 cameraPosition;
};
uniform mat4 modelMatrix;

void main()
{
    gl_Position =  projection * view * modelMatrix * vec4(aPosition, 1.0);
}

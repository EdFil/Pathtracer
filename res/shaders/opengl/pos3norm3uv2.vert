#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;

layout (std140) uniform Matrices {
  mat4 view;
  mat4 projection;
};
uniform mat4 modelMatrix;

out vec2 TexCoords;

void main()
{
    mat4x4 mat = mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);
    TexCoords = aTexCoords;
    gl_Position =  projection * view *  vec4(aPosition, 1.0);
}

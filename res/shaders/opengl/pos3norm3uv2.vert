#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormals;

layout (std140) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec4 cameraPosition;
    float color;
} matrices;

uniform mat4 modelMatrix;

out float Color;
out vec3 Normal;  // Might need to add NormalMatrix logic to this if we want to support scalling correctly
out vec2 TexCoords;
out vec3 WorldPosition; // Used in fragment for light calculations
out vec3 CameraPosition;

void main()
{
    vec4 worldPosition = modelMatrix * vec4(aPosition, 1.0f);
    gl_Position = matrices.projection * matrices.view * worldPosition;

    // Out variables for fragment
    WorldPosition = worldPosition.rgb;
    TexCoords = aTexCoords;
    Normal = aNormals;
    CameraPosition = matrices.cameraPosition.xyz;
    Color = matrices.color;
}

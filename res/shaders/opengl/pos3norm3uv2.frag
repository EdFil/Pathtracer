#version 330 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 WorldPosition;
in vec3 CameraPosition;
out vec4 FragColor;

layout (std140) uniform Light {
    vec4 position;
    vec4 diffuseColor;
    float ambientStrength;
    float power;
} light;

uniform sampler2D Texture01;

const float shininess = 0.0f;
const vec4 specColor = vec4(1.0, 1.0, 1.0, 1.0f);

void main()
{
    vec3 lightDirection = light.position.xyz - WorldPosition;
    float distance = length(lightDirection);
    distance = distance * distance;
    normalize(lightDirection);
    float lightStrength = light.power / distance;

    float lambertian = max(dot(Normal, lightDirection), 0.0f);
    float specular = 0.0f;

    if (lambertian > 0.0) {
        vec3 viewDir = normalize(CameraPosition - WorldPosition);
        vec3 halfDir = normalize(lightDirection + viewDir);
        float specAngle = max(dot(Normal, halfDir), 0.0f);
        specular = pow(specAngle, shininess);
    }

    vec4 textureColor = texture(Texture01, TexCoords);
    vec4 ambient = textureColor * light.ambientStrength;
    vec4 diffuse = textureColor * light.diffuseColor * lambertian * lightStrength;
    vec4 spec = light.diffuseColor * specular * lightStrength;
    FragColor = ambient + diffuse + spec;
}

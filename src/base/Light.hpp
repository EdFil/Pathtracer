#pragma once

#include <glm/vec3.hpp>

class IUniformBuffer;
class RenderingDevice;
class Mesh;

struct LightData {
    glm::vec3 position;
    float padding  = 1.0f;
    glm::vec3 diffuseColor;
    float padding2 = 1.0f;
    float ambientStrength;
    float power;
};

class Light {
public:
    bool init(RenderingDevice& renderingDevice);
    void update(float deltaTime);

    const glm::vec3& position() const { return _lightData.position; }
    const glm::vec3 & diffuseColor() const { return _lightData.diffuseColor; }

private:
    IUniformBuffer* _uniformBuffer;
    LightData _lightData;

    void updateUniformBuffer();
};
#pragma once

#include <glm/vec3.hpp>

class IUniformBuffer;
class RenderingDevice;

class Light {
public:
    bool init(RenderingDevice& renderingDevice);
    void update(float deltaTime);

    const glm::vec3& position() const { return _position; }
    const glm::vec3 & diffuseColor() const { return _diffuseColor; }

private:
    IUniformBuffer* _uniformBuffer;
    glm::vec3 _position;
    glm::vec3 _diffuseColor;

    void updateUniformBuffer();
};
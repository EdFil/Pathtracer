#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
    glm::mat4x4 viewMatrix() const;
    glm::mat4x4 projectionMatrix() const;

private:
    glm::vec3 _position{0.0f, 0.0f, -10.0f};
    glm::vec3 _lookAt{0.0f, 0.0f, 0.0f};
    glm::vec3 _up{0.0f, 1.0f, 0.0f};

    float _fov = 65.0f;
    float _aspectRatio = 4.0f / 3.0f;
    float _near = 1.0f, _far = 100.0f;
};
#include "Camera.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

glm::mat4x4 Camera::viewMatrix() const {
    return glm::lookAt(_position, _lookAt, _up);
}

glm::mat4x4 Camera::projectionMatrix() const {
    return glm::perspective(_fov, _aspectRatio, _near, _far);
}
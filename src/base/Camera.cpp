#include "Camera.hpp"

#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Logger.hpp"
#include "rendering/RenderingDevice.hpp"

namespace {
glm::vec3 k_worldUp{0.0f, 1.0f, 0.0f};
};

bool Camera::init(RenderingDevice& renderingDevice) {
    _buffer = renderingDevice.createBuffer(Buffer::Mode::UniformBlock);
    if (_buffer == nullptr) {
        LOG_ERROR("[Camera] Could not create Uniform buffer object");
        return false;
    }

    _buffer->bind();
    updateUniformBuffer();
    _buffer->updateBufferRange(Buffer::Target::Uniform, 0, 0, 2 * sizeof(glm::mat4x4));
    _buffer->unbind();

    _xRotation = glm::degrees(atan2(_forward.z, _forward.x));
    _yRotation = glm::degrees(asin(-_forward.y));

    return true;
}

void Camera::update(float deltaTime) {
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
    glm::vec3 movementDelta{0.0f};
    if (keyboardState[SDL_SCANCODE_W]) movementDelta.z += 1.0f;
    if (keyboardState[SDL_SCANCODE_S]) movementDelta.z -= 1.0f;
    if (keyboardState[SDL_SCANCODE_D]) movementDelta.x += 1.0f;
    if (keyboardState[SDL_SCANCODE_A]) movementDelta.x -= 1.0f;
    if (keyboardState[SDL_SCANCODE_SPACE]) movementDelta.y += 1.0f;
    if (keyboardState[SDL_SCANCODE_LSHIFT]) movementDelta.y -= 1.0f;

    if (movementDelta.x != 0 || movementDelta.y != 0 || movementDelta.z != 0) {
        _currentSpeed += _acceleration;
        if (_currentSpeed > _maxMovementSpeed) _currentSpeed = _maxMovementSpeed;
        _position += _forward * (movementDelta.z * _currentSpeed * deltaTime);
        _position += _right * (movementDelta.x * _currentSpeed * deltaTime);
        _position.y += movementDelta.y * _currentSpeed * deltaTime;
    } else {
        _currentSpeed = 0.0f;
    }

    int xDiff, yDiff;
    SDL_GetRelativeMouseState(&xDiff, &yDiff);

    const float sensitivity = 0.4f;
    xDiff *= sensitivity;
    yDiff *= sensitivity;

    _xRotation += xDiff * sensitivity;
    _yRotation -= yDiff * sensitivity;

    glm::vec3 direction;
    direction.x = cos(glm::radians(_xRotation)) * cos(glm::radians(_yRotation));
    direction.y = sin(glm::radians(_yRotation));
    direction.z = sin(glm::radians(_xRotation)) * cos(glm::radians(_yRotation));

    _forward = glm::normalize(direction);
    _right = -glm::cross(k_worldUp, _forward);

    updateUniformBuffer();
}

glm::mat4x4 Camera::viewMatrix() const {
    return glm::lookAt(_position, _position + _forward, k_worldUp);
}
glm::mat4x4 Camera::projMatrix() const {
    return glm::perspective(_fov, (float)_width / (float)_height, _near, _far);
}

void Camera::updateUniformBuffer() {
    glm::mat4x4 uniformBlock[2]{viewMatrix(), projMatrix()};
    _buffer->updateData(Buffer::Target::Uniform, Buffer::Usage::Static, uniformBlock, sizeof(uniformBlock));
}
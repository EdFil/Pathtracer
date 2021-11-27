#include "Camera.hpp"

#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <glad/glad.h>
#include "Logger.hpp"
#include "Window.hpp"
#include "rendering/IUniformBuffer.hpp"
#include "rendering/IRenderingDevice.hpp"

namespace {
glm::vec3 k_worldUp{0.0f, 1.0f, 0.0f};

struct UniformData {
    glm::mat4x4 viewMatrix;
    glm::mat4x4 projectionMatrix;
    glm::vec4 cameraPosition;
};

};

static unsigned int uboMatrices;

bool Camera::init(const Window& window, IRenderingDevice& renderingDevice) {
    _uniformBuffer = renderingDevice.uniformBufferManager()->createUniformBuffer(0, sizeof(UniformData));
    if (_uniformBuffer == nullptr) {
        LOG_ERROR("[Camera] Could not create Uniform buffer object");
        return false;
    }

    _window = &window;
    reset();

    return true;
}

void Camera::update(float deltaTime) {
    int xDiff, yDiff;
    SDL_GetRelativeMouseState(&xDiff, &yDiff);

    const bool isMouseCurrentlyGrabbed = _window->isMouseGrabbed();
    const bool shouldUpdateCamera = isMouseCurrentlyGrabbed && _wasMouseGrabbedLastFrame;
    _wasMouseGrabbedLastFrame = isMouseCurrentlyGrabbed;
    if (!shouldUpdateCamera) return;

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

    const float sensitivity = 0.2f;

    _xRotation += float(xDiff) * sensitivity;
    _yRotation -= float(yDiff) * sensitivity;

    glm::vec3 direction;
    direction.x = cos(glm::radians(_xRotation)) * cos(glm::radians(_yRotation));
    direction.y = sin(glm::radians(_yRotation));
    direction.z = sin(glm::radians(_xRotation)) * cos(glm::radians(_yRotation));

    _forward = glm::normalize(direction);
    _right = -glm::cross(k_worldUp, _forward);

    updateUniformBuffer();
}

void Camera::bind() {
    _uniformBuffer->bind();
}

void Camera::reset() {
    _position = {0.0f, 0.0f, 3.0f};
    _forward = {0.0f, 0.0f, -1.0f};
    _right = {1.0f, 0.0f, 0.0f};
    _fov = 45.0f;
    _xRotation = glm::degrees(atan2(_forward.z, _forward.x));
    _yRotation = glm::degrees(asin(-_forward.y));
    updateUniformBuffer();
}

glm::mat4x4 Camera::viewMatrix() const {
    return glm::lookAt(_position, _position + _forward, k_worldUp);
}
glm::mat4x4 Camera::projMatrix() const {
    return glm::perspective(_fov, (float)_width / (float)_height, _near, _far);
}

void Camera::updateUniformBuffer() {
    UniformData uniformData = {viewMatrix(), projMatrix(), glm::vec4(_position, 1.0f)};
    _uniformBuffer->updateData(&uniformData, sizeof(UniformData));

}
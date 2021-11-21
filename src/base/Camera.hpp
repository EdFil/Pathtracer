#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class IUniformBuffer;
class IRenderingDevice;
class Window;

class Camera {
public:
    bool init(const Window& window, IRenderingDevice& renderingDevice);
    void update(float deltaTime);
    void reset();

    const glm::vec3& position() const { return _position; };
    const glm::vec3& forward() const { return _forward; };
    glm::mat4x4 viewMatrix() const;
    glm::mat4x4 projMatrix() const;

private:
    IUniformBuffer* _uniformBuffer;
    const Window* _window;
    glm::vec3 _position{0.0f, 0.0f, 3.0f};

    // Mouse state
    bool _wasMouseGrabbedLastFrame = false;

    // Updated every frame and used to calculate forward vector. Using Euler representation (ZYX)
    float _xRotation = 0.0f;
    float _yRotation = 0.0f;

    glm::vec3 _forward{0.0f, 0.0f, -1.0f};
    glm::vec3 _right{1.0f, 0.0f, 0.0f};
    float _fov = 45.0f;
    int _width = 640;
    int _height = 480;
    float _near = 0.01f;
    float _far = 100.0f;

    float _currentSpeed = 0.0f;
    float _acceleration = 0.3f;
    float _maxMovementSpeed = 5.0f;

    void updateUniformBuffer();
};
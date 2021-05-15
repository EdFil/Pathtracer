#include "Light.hpp"

#include <cmath>

#include "Logger.hpp"
#include "rendering/IUniformBuffer.h"
#include "rendering/RenderingDevice.hpp"

bool Light::init(RenderingDevice& renderingDevice) {
    _uniformBuffer = renderingDevice.createUniformBuffer(1, 2 * sizeof(glm::vec3));
    if (_uniformBuffer == nullptr) {
        LOG_ERROR("[Light] Could not create Uniform buffer object");
        return false;
    }

    _position = {0.0f, 0.0f, 0.0f};
    _diffuseColor = {0.0f, 0.0f, 1.0f};
    updateUniformBuffer();

    return true;
}

void Light::update(float deltaTime) {
    updateUniformBuffer();
}

void Light::updateUniformBuffer() {
    glm::vec3 uniformBlock[2]{_position, _diffuseColor};
    _uniformBuffer->updateData(uniformBlock, sizeof(uniformBlock));
}
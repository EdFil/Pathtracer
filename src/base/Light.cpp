#include "Light.hpp"

#include <cmath>
#include <glm/gtx/rotate_vector.hpp>

#include "Logger.hpp"
#include "rendering/IUniformBuffer.h"
#include "rendering/IRenderingDevice.hpp"

bool Light::init(IRenderingDevice& renderingDevice) {
    _uniformBuffer = renderingDevice.createUniformBuffer(1, sizeof(LightData));
    if (_uniformBuffer == nullptr) {
        LOG_ERROR("[Light] Could not create Uniform buffer object");
        return false;
    }

    _lightData.position = {0.0f, 0.0f, 10.0f};
    _lightData.diffuseColor = {1.0f, 0.0f, 0.0f};
    _lightData.ambientStrength = 0.1f;
    _lightData.power = 10.0f;
    updateUniformBuffer();

    return true;
}

void Light::update(float deltaTime) {
    _lightData.position = glm::rotateY(_lightData.position, glm::radians(180.0f) * deltaTime);
    static float accumulator = 0;
    accumulator += deltaTime;
    _lightData.diffuseColor = {cosf(accumulator), sinf(accumulator * 0.5f), sinf(accumulator * 5.0f)};
    updateUniformBuffer();
}

void Light::updateUniformBuffer() {
    _uniformBuffer->updateData(&_lightData, sizeof(LightData));
}
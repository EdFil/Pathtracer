#pragma once

#include "rendering/IUniformBuffer.h"

class UniformBufferGL final : public IUniformBuffer {
public:
    bool init(unsigned int bindingPoint, unsigned int sizeInBytes) override;
    bool updateData(const void* data, unsigned int sizeInBytes) override;

private:
    unsigned int _bindingPoint = 0;
    unsigned int _handle = 0;
};
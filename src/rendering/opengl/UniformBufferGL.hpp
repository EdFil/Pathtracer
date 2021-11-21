#pragma once

#include "rendering/IUniformBuffer.hpp"

class UniformBufferGL final : public IUniformBuffer {
public:
    virtual bool init(unsigned int bindingPoint, unsigned int sizeInBytes) override;
    virtual bool updateData(const void* data, unsigned int sizeInBytes) override;
    virtual void bind() override;

private:
    unsigned int _bindingPoint = 0;
    unsigned int _handle = 0;
};
#pragma once

#include <cstdint>

class IUniformBuffer {
public:
    virtual ~IUniformBuffer() = default;

    virtual bool init(uint32_t bindingPoint, uint32_t sizeInBytes) = 0;
    virtual bool updateData(const void* data, uint32_t sizeInBytes) = 0;
    virtual void bind() = 0;
    virtual uint32_t handle() const = 0;
};

class IUniformBufferManager {
public:
    virtual ~IUniformBufferManager() = default;

    virtual IUniformBuffer* createUniformBuffer(uint32_t bindingPoint, uint32_t sizeInBytes) = 0;
    virtual void deleteUniformBuffer(IUniformBuffer* uniformBuffer) = 0;
};
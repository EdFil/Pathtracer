#pragma once

#include <EASTL/map.h>
#include <EASTL/unique_ptr.h>

#include "rendering/IUniformBuffer.hpp"

class UniformBufferGL final : public IUniformBuffer {
public:
    ~UniformBufferGL() override = default;

    virtual bool init(uint32_t bindingPoint, uint32_t sizeInBytes) override;
    virtual bool updateData(const void* data, uint32_t sizeInBytes) override;
    virtual void bind() override;
    virtual uint32_t handle() const override { return _handle; };

private:
    unsigned int _bindingPoint = 0;
    unsigned int _handle = 0;
    unsigned int _sizeInBytes = 0;
};

class UniformBufferManagerGL final : public IUniformBufferManager {
public:
    virtual IUniformBuffer* createUniformBuffer(uint32_t bindingPoint, uint32_t sizeInBytes) override;
    virtual void deleteUniformBuffer(IUniformBuffer* uniformBuffer) override;

private:
    eastl::map<int32_t, eastl::unique_ptr<UniformBufferGL>> _buffers;
};
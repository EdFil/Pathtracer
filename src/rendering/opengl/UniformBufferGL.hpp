#pragma once

#include <map>
#include <memory>

#include "rendering/IUniformBuffer.hpp"

class UniformBufferGL final : public IUniformBuffer {
public:
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
    std::map<int32_t, std::unique_ptr<UniformBufferGL>> _buffers;
};
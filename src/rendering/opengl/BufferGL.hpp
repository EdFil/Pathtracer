#pragma once

#include <EASTL/map.h>
#include <EASTL/unique_ptr.h>

#include "rendering/IBuffer.hpp"

class BufferGL final : public IBuffer {
public:
    ~BufferGL() override;

    int handle() const override { return _handle; }

    virtual void init(Buffer::Mode mode) override;
    virtual bool updateData(Buffer::Target target, Buffer::Usage usage, const void* data, uint32_t sizeInBytes) override;
    virtual void updateAttribute(uint32_t index, uint32_t size, uint32_t stride, uint32_t dataBegin) override;
    virtual void bind() override;
    virtual void unbind() override;
    virtual void draw(unsigned int count) override;

private:
    unsigned int _bufferObjects[2];
    unsigned int _handle = 0;
    Buffer::Mode _mode = Buffer::Mode::Undefined;

    void generateBuffersFor(Buffer::Mode mode);
    void clearBuffersFor(Buffer::Mode mode);
    bool isTargetCompatibleWithCurrentMode(Buffer::Target target) const;
    unsigned int bufferForTarget(Buffer::Target target);
};


class BufferManagerGL final : public IBufferManager {
public:
    IBuffer* createBuffer(const Buffer::Mode& mode);
    IBuffer* buffer(uint32_t handle) const;

private:
    eastl::map<uint32_t, eastl::unique_ptr<BufferGL>> _buffers;
};
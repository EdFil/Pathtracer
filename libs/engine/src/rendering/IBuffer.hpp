#pragma once

#include <cstdint>

namespace Buffer {
    enum class Mode { Undefined = 0, Vertex, VertexElementPair };
    enum class Target { Array = 0, Element };
    enum class Usage { Static = 0, Stream, Dynamic };
}

class IBuffer {
public:
    virtual ~IBuffer() = default;

    virtual int handle() const = 0;

    virtual void init(Buffer::Mode mode) = 0;
    virtual bool updateData(Buffer::Target target, Buffer::Usage usage, const void* data, uint32_t sizeInBytes) = 0;
    virtual void updateAttribute(unsigned int index, unsigned int size, unsigned int stride, unsigned int dataBegin) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual void draw(unsigned int count) = 0;
};

class IBufferManager {
public:
    virtual ~IBufferManager() = default;

    virtual IBuffer* createBuffer(const Buffer::Mode& mode) = 0;
    virtual IBuffer* buffer(uint32_t handle) const = 0;
};
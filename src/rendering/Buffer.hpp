#pragma once

#include <cstdint>

class Buffer {
public:
    enum class Mode { Undefined = 0, Vertex, VertexElementPair };
    enum class Target { Array = 0, Element };
    enum class Usage { Static = 0, Stream, Dynamic };

    virtual ~Buffer() = default;

    virtual void init(Mode mode) = 0;
    virtual bool updateData(Target target, Usage usage, const void* data, uint32_t sizeInBytes) = 0;
    virtual void updateAttribute(unsigned int index, unsigned int size, unsigned int stride, unsigned int dataBegin) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual void draw(unsigned int count) = 0;
    unsigned int id() const { return _handle; };

protected:
    unsigned int _handle = 0;
    Mode _mode = Mode::Undefined;
};
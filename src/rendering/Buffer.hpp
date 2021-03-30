#pragma once

#include <cstdint>

class Buffer {
public:
    enum class Type : uint8_t { Array, Indices };
    enum class Usage : uint8_t { Stream, Static, Dynamic };

    Buffer(Type type, Usage usage) : _type(type), _usage(usage) {}
    virtual ~Buffer() = default;

    virtual void updateData(void* data, uint32_t size) = 0;

protected:
    Type _type;
    Usage _usage;
};
#pragma once

#include <cstdint>

class Buffer {
public:
    enum class Type { Array = 0, Indices, Constant };
    enum class Usage { Static = 0, Stream, Dynamic };

    struct Params {
        Buffer::Type type = Buffer::Type::Array;
        Buffer::Usage usage = Buffer::Usage::Static;
    };

    virtual ~Buffer() = default;

    virtual bool init(const Buffer::Params& params) = 0;
    virtual bool updateData(void* data, uint32_t size) = 0;
	virtual void updateAttribute(uint32_t index, uint32_t size, uint32_t stride, uint32_t dataBegin) = 0;
	virtual void bind() = 0;
    virtual uint32_t id() const { return _handle; };

protected:
    unsigned int _handle = 0;
    Type _type{Type::Array};
    Usage _usage{Usage::Static};
};
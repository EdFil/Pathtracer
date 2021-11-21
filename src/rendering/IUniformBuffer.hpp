#pragma once

class IUniformBuffer {
public:
    virtual ~IUniformBuffer() = default;

    virtual bool init(unsigned int bindingPoint, unsigned int sizeInBytes) = 0;
    virtual bool updateData(const void* data, unsigned int sizeInBytes) = 0;
    virtual void bind() = 0;
};
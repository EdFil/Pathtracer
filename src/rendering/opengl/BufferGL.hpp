#pragma once

#include "rendering/Buffer.hpp"

class BufferGL final : public Buffer {
public:
    ~BufferGL() override;

    void init(Mode mode) override;
    bool updateData(Target target, Usage usage, const void* data, uint32_t sizeInBytes) override;
    void updateBufferRange(Target target, unsigned int index, unsigned int offset, unsigned int size) override;
    void updateAttribute(uint32_t index, uint32_t size, uint32_t stride, uint32_t dataBegin) override;

    void bind() override;
    void unbind() override;
    void draw(unsigned int count) override;

private:
    unsigned int _bufferObjects[2];

    void generateBuffersFor(Mode mode);
    void clearBuffersFor(Mode mode);
    bool isTargetCompatibleWithCurrentMode(Target target) const;
    unsigned int bufferForTarget(Target target);
};
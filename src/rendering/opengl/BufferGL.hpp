#pragma once

#include "rendering/Buffer.hpp"

class BufferGL final : public Buffer {
public:
    ~BufferGL();

    bool init(const Buffer::Params& params) override;
    bool updateData(void* data, uint32_t size) override;
	void updateAttribute(uint32_t index, uint32_t size, uint32_t stride, uint32_t dataBegin) override;

    void bind() override;

private:
    unsigned int _vbo = 0;
    unsigned int _ibo = 0;
};
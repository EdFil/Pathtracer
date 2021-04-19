#pragma once

#include "rendering/Buffer.hpp"

#include <cstdint>

class BufferGL final : public Buffer {
public:
    ~BufferGL();

    bool init(const Buffer::Params& params) override;
    bool updateData(void* data, uint32_t size) override;
    void bind() override;
    uint32_t id() const override { return _vao; }

private:
    uint32_t _vao = 0;
    uint32_t _vbo = 0;
};
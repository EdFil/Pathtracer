#pragma once

#include <map>
#include <memory>

#include "rendering/Buffer.hpp"

class BufferGL;

class BufferManagerGL {
public:
    BufferManagerGL();
    ~BufferManagerGL();

    Buffer* createBuffer(const Buffer::Mode& mode);
    Buffer* buffer(uint32_t id) const;

private:
    std::map<uint32_t, std::unique_ptr<BufferGL>> _buffers;
};
#pragma once

#include <map>

#include "rendering/opengl/BufferGL.hpp"

class BufferManagerGL {
public:
    Buffer* createBuffer(const Buffer::Mode& mode);
    Buffer* buffer(uint32_t id) const;

private:
    std::map<uint32_t, BufferGL*> _buffers;
};
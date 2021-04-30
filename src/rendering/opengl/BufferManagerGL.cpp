#include "rendering/opengl/BufferManagerGL.hpp"

Buffer* BufferManagerGL::createBuffer(const Buffer::Mode& mode) {
    BufferGL* buffer = new BufferGL();
    buffer->init(mode);

    _buffers[buffer->id()] = buffer;
    return buffer;
}

Buffer* BufferManagerGL::buffer(uint32_t id) const {
    const auto it = _buffers.find(id);
    if (it == _buffers.cend()) return nullptr;

    return it->second;
}

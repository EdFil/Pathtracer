#include "rendering/opengl/BufferManagerGL.hpp"

#include "BufferGL.hpp"

BufferManagerGL::BufferManagerGL() = default;
BufferManagerGL::~BufferManagerGL() = default;

Buffer* BufferManagerGL::createBuffer(const Buffer::Mode& mode) {
    std::unique_ptr<BufferGL> buffer = std::make_unique<BufferGL>();
    buffer->init(mode);

    const auto it = _buffers.insert({buffer->id(), std::move(buffer)});
    return it.first->second.get();
}

Buffer* BufferManagerGL::buffer(uint32_t id) const {
    const auto it = _buffers.find(id);
    if (it == _buffers.cend()) return nullptr;

    return it->second.get();
}

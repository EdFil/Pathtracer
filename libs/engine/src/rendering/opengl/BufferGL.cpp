#include "rendering/opengl/BufferGL.hpp"

#include <glad/glad.h>
#include <cstring>

#include <cassert>
#include "Logger.hpp"

namespace {
constexpr GLenum toGLUsage(Buffer::Usage usage) {
    switch (usage) {
        case Buffer::Usage::Stream:
            return GL_STREAM_DRAW;
        case Buffer::Usage::Static:
            return GL_STATIC_DRAW;
        case Buffer::Usage::Dynamic:
            return GL_DYNAMIC_DRAW;
        default:
            assert(false);
    }
}

constexpr GLenum toGLTarget(Buffer::Target target) {
    switch (target) {
        case Buffer::Target::Array:
            return GL_ARRAY_BUFFER;
        case Buffer::Target::Element:
            return GL_ELEMENT_ARRAY_BUFFER;
        default:
            assert(false);
    }
}
}  // namespace

BufferGL::~BufferGL() {
    clearBuffersFor(_mode);
}

void BufferGL::init(Buffer::Mode mode) {
    if (_mode == mode) {
        return;
    }

    clearBuffersFor(_mode);
    generateBuffersFor(mode);
    _mode = mode;
}

bool BufferGL::updateData(Buffer::Target target, Buffer::Usage usage, const void *data, uint32_t sizeInBytes) {
    if (data == nullptr && !isTargetCompatibleWithCurrentMode(target)) {
        return false;
    }

    unsigned int bufferObject = bufferForTarget(target);
    if (bufferObject == 0) {
        return false;
    }

    const GLenum glTarget = toGLTarget(target);
    glBindBuffer(glTarget, bufferObject);
    glBufferData(glTarget, sizeInBytes, data, toGLUsage(usage));
    return true;
}

void BufferGL::updateAttribute(uint32_t index, uint32_t size, uint32_t stride, uint32_t dataBegin) {
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void *)(dataBegin * sizeof(float)));
    glEnableVertexAttribArray(index);
}

void BufferGL::bind() {
    glBindVertexArray(_handle);
}

void BufferGL::unbind() {
    glBindVertexArray(0);
}

void BufferGL::draw(unsigned int count) {
    if (_mode == Buffer::Mode::Vertex) {
        glDrawArrays(GL_TRIANGLES, 0, count);
    } else {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    }
}

void BufferGL::generateBuffersFor(Buffer::Mode mode) {
    glGenVertexArrays(1, &_handle);
    switch (mode) {
        case Buffer::Mode::Vertex:
            glGenBuffers(1, _bufferObjects);
            break;
        case Buffer::Mode::VertexElementPair:
            glGenBuffers(2, _bufferObjects);
            break;
        default:
            LOG_ERROR("[BufferGL] Mode %d is not handled", _mode);
            break;
    }
}

void BufferGL::clearBuffersFor(Buffer::Mode mode) {
    switch (mode) {
        case Buffer::Mode::Vertex:
            glDeleteVertexArrays(1, &_handle);
            glDeleteBuffers(1, _bufferObjects);
        case Buffer::Mode::VertexElementPair:
            glDeleteVertexArrays(1, &_handle);
            glDeleteBuffers(2, _bufferObjects);
            break;
        default:
            break;
    }
}

bool BufferGL::isTargetCompatibleWithCurrentMode(Buffer::Target target) const {
    switch (_mode) {
        case Buffer::Mode::Vertex:
            return target == Buffer::Target::Array;
        case Buffer::Mode::VertexElementPair:
            return target == Buffer::Target::Array || target == Buffer::Target::Element;
        default:
            LOG_ERROR("[BufferGL] Mode %d is not handled", _mode);
            return false;
    }
}

unsigned int BufferGL::bufferForTarget(Buffer::Target target) {
    switch (target) {
        case Buffer::Target::Array:
            return _bufferObjects[0];
        case Buffer::Target::Element:
            return _bufferObjects[1];
        default:
            LOG_ERROR("[BufferGL] Target %d is not handled", _mode);
            return 0;
    }
}

// ---------------------------------------------
// -------------  BufferManagerGL  -------------
// ---------------------------------------------

IBuffer* BufferManagerGL::createBuffer(const Buffer::Mode& mode) {
    eastl::unique_ptr<BufferGL> buffer = eastl::make_unique<BufferGL>();
    buffer->init(mode);

    const auto it = _buffers.emplace(buffer->handle(), eastl::move(buffer));
    return it.first->second.get();
}

IBuffer* BufferManagerGL::buffer(uint32_t handle) const {
    const auto it = _buffers.find(handle);
    if (it == _buffers.cend()) return nullptr;

    return it->second.get();
}

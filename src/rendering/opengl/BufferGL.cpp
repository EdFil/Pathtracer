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

void BufferGL::init(Mode mode) {
    if (_mode == mode) {
        return;
    }

    clearBuffersFor(_mode);
    generateBuffersFor(mode);
    _mode = mode;
}

bool BufferGL::updateData(Target target, Usage usage, const void *data, uint32_t sizeInBytes) {
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

void BufferGL::generateBuffersFor(Mode mode) {
    glGenVertexArrays(1, &_handle);
    switch (mode) {
        case Mode::Vertex:
            glGenBuffers(1, _bufferObjects);
            break;
        case Mode::VertexElementPair:
            glGenBuffers(2, _bufferObjects);
            break;
        default:
            LOG_ERROR("[BufferGL] Mode %d is not handled", _mode);
            break;
    }
}

void BufferGL::clearBuffersFor(Mode mode) {
    switch (mode) {
        case Mode::Vertex:
            glDeleteVertexArrays(1, &_handle);
            glDeleteBuffers(1, _bufferObjects);
        case Mode::VertexElementPair:
            glDeleteVertexArrays(1, &_handle);
            glDeleteBuffers(2, _bufferObjects);
            break;
        default:
            break;
    }
}

bool BufferGL::isTargetCompatibleWithCurrentMode(Target target) const {
    switch (_mode) {
        case Mode::Vertex:
            return target == Target::Array;
        case Mode::VertexElementPair:
            return target == Target::Array || target == Target::Element;
        default:
            LOG_ERROR("[BufferGL] Mode %d is not handled", _mode);
            return false;
    }
}

unsigned int BufferGL::bufferForTarget(Target target) {
    switch (target) {
        case Target::Array:
            return _bufferObjects[0];
        case Target::Element:
            return _bufferObjects[1];
        default:
            LOG_ERROR("[BufferGL] Target %d is not handled", _mode);
            return 0;
    }
}

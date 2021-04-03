#include "rendering/opengl/BufferGL.hpp"

#include <glad/glad.h>
#include <cstring>

GLenum toGLUsage(Buffer::Usage usage) {
    switch (usage) {
        case Buffer::Usage::Stream:
            return GL_STREAM_DRAW;
        case Buffer::Usage::Static:
            return GL_STATIC_DRAW;
        case Buffer::Usage::Dynamic:
        default:
            return GL_DYNAMIC_DRAW;
    }
}

BufferGL::BufferGL(Type type, Usage usage) : Buffer(type, usage) {
    glGenBuffers(1, _buffer);
}

BufferGL::~BufferGL() {
    if (_buffer != nullptr) {
        glDeleteBuffers(1, _buffer);
    }
}

void BufferGL::updateData(void* data, uint32_t size) {
    if (_buffer) {
        if (_type == Type::Array) {
            glBindBuffer(GL_ARRAY_BUFFER, *_buffer);
            glBufferData(GL_ARRAY_BUFFER, size, data, toGLUsage(_usage));
        } else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *_buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, toGLUsage(_usage));
        }
    }
}
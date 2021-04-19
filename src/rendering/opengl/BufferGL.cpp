#include "rendering/opengl/BufferGL.hpp"

#include <glad/glad.h>
#include <cstring>

#include "Logger.hpp"

namespace {
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
}  // namespace

void updateAttributeLayouts(Buffer::DataLayout layout) {
    switch (layout) {
        case Buffer::DataLayout::Pos3:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
            glEnableVertexAttribArray(0);
            break;
        default:
            break;
    }
}

BufferGL::~BufferGL() {
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);

    if (_vbo != 0) glDeleteBuffers(1, &_vbo);
}

bool BufferGL::init(const Buffer::Params& params) {
    if (_vao == 0) glGenVertexArrays(1, &_vao);
    if (_vbo == 0) glGenBuffers(1, &_vbo);

    _layout = params.layout;
    _type = params.type;
    _usage = params.usage;

    if (!updateData(params.data, params.size)) return false;

    updateAttributeLayouts(_layout);
    return true;
}

bool BufferGL::updateData(void* data, uint32_t size) {
    if (_vao == 0 || _vbo == 0) {
        LOG_ERROR("[BufferGL] Trying to call updateData without valid VAO or VBO. Failing...");
        return false;
    }

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, toGLUsage(_usage));

    return true;
}

void BufferGL::bind() {
    glBindVertexArray(_vao);
}
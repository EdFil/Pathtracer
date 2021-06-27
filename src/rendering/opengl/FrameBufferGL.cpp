#include "FrameBufferGL.hpp"

#include <glad/glad.h>

#include "Logger.hpp"
#include "rendering/opengl/TextureGL.hpp"

namespace {
constexpr GLenum attachmentToGL(IFrameBuffer::Attachment attachment);
}

FrameBufferGL::~FrameBufferGL() {
    destroy();
}

bool FrameBufferGL::init(const char* name) {
    if (_handle != 0) {
        LOG_ERROR("[FrameBufferGL] Buffer was already initialised. Handle(%d) Name(%s)", _handle, _name.c_str());
        return false;
    }

    glGenFramebuffers(1, &_handle);
    _name = std::string(name);
    bool wasSuccess = _handle != 0 && !hasFramebufferError();
    if (wasSuccess) {
        LOG("[FrameBufferGL] Init successful. Handle(%d) Name(%s)", _handle, _name.c_str());
    }

    return wasSuccess;
}

void FrameBufferGL::destroy() {
    if (_handle != 0) {
        LOG("[FrameBufferGL] Destroy Handle(%d) Name(%s)", _handle, _name.c_str());
        glDeleteFramebuffers(1, &_handle);
        _handle = 0;
        _name.clear();
    }
}

bool FrameBufferGL::attachTexture(Texture* texture, Attachment attachment) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentToGL(attachment), GL_TEXTURE_2D, texture->handle(), 0);
    return hasFramebufferError();
}

void FrameBufferGL::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, _handle);
}

void FrameBufferGL::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FrameBufferGL::hasFramebufferError() {
    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
        case GL_FRAMEBUFFER_COMPLETE:
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            LOG_ERROR("[FrameBufferGL] (%s) - GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT", _name.c_str());
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            LOG_ERROR("[FrameBufferGL] (%s) The framebuffer attachment points are framebuffer incomplete", _name.c_str());
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            LOG_ERROR("[FrameBufferGL] (%s) GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER", _name.c_str());
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            LOG_ERROR("[FrameBufferGL] (%s) GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER", _name.c_str());
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            LOG_ERROR("[FrameBufferGL] (%s) GL_FRAMEBUFFER_UNSUPPORTED", _name.c_str());
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            LOG_ERROR("[FrameBufferGL] (%s) GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE", _name.c_str());
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            LOG_ERROR("[FrameBufferGL] (%s) GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE", _name.c_str());
            break;
        default:
            LOG_ERROR("[FrameBufferGL] (%s) Error not handled %d", _name.c_str(), (int)status);
            break;
    }
    return true;
}

// Anonymous functions

namespace {
constexpr GLenum attachmentToGL(IFrameBuffer::Attachment attachment) {
    switch (attachment) {
        case IFrameBuffer::Attachment::Color:
            return GL_COLOR_ATTACHMENT0;
        case IFrameBuffer::Attachment::Depth:
            return GL_DEPTH_ATTACHMENT;
        case IFrameBuffer::Attachment::Stencil:
            return GL_STENCIL_ATTACHMENT;
        case IFrameBuffer::Attachment::DepthStencil:
            return GL_DEPTH_STENCIL_ATTACHMENT;
    }
}
}  // namespace
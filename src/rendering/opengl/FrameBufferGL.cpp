#include "FrameBufferGL.hpp"

#include <glad/glad.h>

#include "Logger.hpp"
#include "TextureGL.hpp"

namespace {
    constexpr char kDefaultBufferName[] = "Default";

    constexpr GLenum attachmentToGL(IFrameBuffer::Attachment attachment) {
        switch (attachment) {
            case IFrameBuffer::Attachment::Color0:
                return GL_COLOR_ATTACHMENT0;
            case IFrameBuffer::Attachment::Depth:
                return GL_DEPTH_ATTACHMENT;
            case IFrameBuffer::Attachment::Stencil:
                return GL_STENCIL_ATTACHMENT;
            case IFrameBuffer::Attachment::DepthStencil:
                return GL_DEPTH_STENCIL_ATTACHMENT;
            default:
                LOG_ERROR("[FrameBufferGL] Unhandled attrachment %d", attachment);
                return GL_INVALID_ENUM;
        }
    }
}  // namespace

// -------------------------------------------
// -------------  FrameBufferGL  -------------
// -------------------------------------------

FrameBufferGL::~FrameBufferGL() {
    destroy();
}

bool FrameBufferGL::init(std::string&& name) {
    if (_handle != 0) {
        LOG_ERROR("[FrameBufferGL] Init %s failed. Buffer %d already initialised", _name.c_str(), _handle);
        return false;
    }

    glGenFramebuffers(1, &_handle);
    _name = std::move(name);
    bool wasSuccess = _handle != 0 && !hasFramebufferError();
    if (wasSuccess) {
        LOG("[FrameBufferGL] Init successful. Handle(%d) Name(%s)", _handle, _name.c_str());
    }

    return wasSuccess;
}

bool FrameBufferGL::init(uint32_t handle, std::string&& name) {    
    if (handle != 0 && glIsFramebuffer(handle) == GL_FALSE) {
        LOG_ERROR("[FrameBufferGL] Init %s failed. %d is not a valid framebuffer object handle", name.c_str(), handle);
        return false;
    }

    _handle = handle;
    _name = std::move(name);
    LOG("[FrameBufferGL] Init successful. Handle(%d) Name(%s)", _handle, _name.c_str());
    return !hasFramebufferError();
}

void FrameBufferGL::destroy() {
    if (_handle != 0) {
        LOG("[FrameBufferGL] Destroy Handle(%d) Name(%s)", _handle, _name.c_str());
        glDeleteFramebuffers(1, &_handle);
        _handle = 0;
        _name.clear();
    }
}

bool FrameBufferGL::attachTexture(ITexture* texture, Attachment attachment) {
    if (texture == nullptr) {
        LOG_ERROR("[FrameBufferGL] Cannot attach to a null texture. Texture(%p) Attachment(%d)", texture, attachment);
        return false;
    }

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentToGL(attachment), GL_TEXTURE_2D, texture->handle(), 0);
    unbind();
    return !hasFramebufferError();
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

// --------------------------------------------------
// -------------  FrameBufferManagerGL  -------------
// --------------------------------------------------

bool FrameBufferManagerGL::init() {
    std::unique_ptr<FrameBufferGL> frameBuffer = std::make_unique<FrameBufferGL>();
    if (frameBuffer == nullptr || !frameBuffer->init(0, "Default")) {
        LOG_ERROR("[FrameBufferManagerGL] Failed to create \"Default\" FrameBuffer. FrameBuffer(%p)", frameBuffer.get());
        return false;
    }

    const auto it = _frameBuffers.insert({frameBuffer->handle(), std::move(frameBuffer)});
    if (!it.second) {
        LOG_ERROR("[FrameBufferManagerGL] Failed to insert \"Default\" framebuffer into map. Memory allocation failed?");
        return false;
    }

    return true;
}

IFrameBuffer* FrameBufferManagerGL::createFrameBuffer(std::string&& name) {
    std::unique_ptr<FrameBufferGL> frameBuffer = std::make_unique<FrameBufferGL>();
    if (frameBuffer == nullptr || !frameBuffer->init(std::move(name))) {
        LOG_ERROR("[FrameBufferManagerGL] Failed to create FrameBuffer. FrameBuffer(%p)", frameBuffer.get());
        return nullptr;
    }

    const auto it = _frameBuffers.insert({frameBuffer->handle(), std::move(frameBuffer)});
    if (!it.second) {
        LOG_ERROR("[FrameBufferManagerGL] Failed to insert framebuffer into map. Memory allocation failed?");
        return nullptr;
    }

    return it.first->second.get();
}

IFrameBuffer* FrameBufferManagerGL::frameBuffer(const std::string& name) const {
    for (const auto& pair : _frameBuffers) {
        if (pair.second->name() == name) {
            return pair.second.get();
        }
    }

    return nullptr;
}

IFrameBuffer* FrameBufferManagerGL::frameBuffer(uint32_t handle) const {
    const auto it = _frameBuffers.find(handle);
    return it == _frameBuffers.cend() ? nullptr : it->second.get();
}
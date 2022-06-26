#pragma once

#include <EASTL/string.h>
#include <cstdint>

class ITexture;

class IFrameBuffer {
public:
    enum class Attachment { Color0, Depth, Stencil, DepthStencil };

    virtual ~IFrameBuffer() = default;

    virtual uint32_t handle() const = 0;
    virtual const eastl::string& name() = 0;

    virtual bool init(eastl::string&& name) = 0;
    virtual bool init(uint32_t handle, eastl::string&& name) = 0;
    virtual void destroy() = 0;
    virtual ITexture* texture() = 0;
    virtual bool attachTexture(ITexture* texture, Attachment attachment) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual bool resize(int width, int height) = 0;
};

class IFrameBufferManager {
public:
    virtual ~IFrameBufferManager() = default;

    virtual bool init() = 0;
    virtual IFrameBuffer* createFrameBuffer(eastl::string&& name) = 0;
    virtual IFrameBuffer* frameBuffer(const eastl::string& name) const = 0;
    virtual IFrameBuffer* frameBuffer(uint32_t handle) const = 0;
};

#pragma once

#include <string>
#include <cstdint>

class ITexture;

class IFrameBuffer {
public:
    enum class Attachment { Color0, Depth, Stencil, DepthStencil };

    virtual ~IFrameBuffer() = default;

    virtual uint32_t id() const = 0;
    virtual const std::string& name() = 0;

    virtual bool init(std::string&& name) = 0;
    virtual bool init(uint32_t id, std::string&& name) = 0;
    virtual void destroy() = 0;
    virtual bool attachTexture(ITexture* texture, Attachment attachment) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;
};

class IFrameBufferManager {
public:
    virtual ~IFrameBufferManager() = default;

    virtual bool init() = 0;
    virtual IFrameBuffer* createFrameBuffer(std::string&& name) = 0;
    virtual IFrameBuffer* frameBuffer(const std::string& name) const = 0;
    virtual IFrameBuffer* frameBuffer(uint32_t id) const = 0;
};

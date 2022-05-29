#pragma once

#include <EASTL/map.h>
#include <EASTL/unique_ptr.h>

#include "rendering/IFrameBuffer.hpp"

class FrameBufferGL final : public IFrameBuffer {
public:
    ~FrameBufferGL() override;

    virtual uint32_t handle() const override { return _handle; };
    virtual const eastl::string& name() override { return _name; };

    bool init(eastl::string&& name) override;
    bool init(uint32_t handle, eastl::string&& name) override;
    void destroy() override;
    bool attachTexture(ITexture *texture, Attachment attachment) override;
    void bind() override;
    void unbind() override;

private:
    unsigned int _handle;
    eastl::string _name;

    bool hasFramebufferError();
};

class FrameBufferManagerGL final : public IFrameBufferManager {
public:
    virtual ~FrameBufferManagerGL() override = default;

    virtual bool init() override;
    virtual IFrameBuffer *createFrameBuffer(eastl::string&& name) override;
    virtual IFrameBuffer *frameBuffer(const eastl::string& name) const override;
    virtual IFrameBuffer *frameBuffer(uint32_t handle) const override;

private:
    eastl::map<uint32_t, eastl::unique_ptr<FrameBufferGL>> _frameBuffers;
};
#pragma once

#include <map>
#include <memory>

#include "rendering/IFrameBuffer.hpp"

class FrameBufferGL final : public IFrameBuffer {
public:
    ~FrameBufferGL() override;

    virtual uint32_t id() const override { return _handle; };
    virtual const std::string& name() override { return _name; };

    bool init(std::string&& name) override;
    bool init(uint32_t id, std::string&& name) override;
    void destroy() override;
    bool attachTexture(ITexture *texture, Attachment attachment) override;
    void bind() override;
    void unbind() override;

private:
    unsigned int _handle;
    std::string _name;

    bool hasFramebufferError();
};

class FrameBufferManagerGL final : public IFrameBufferManager {
public:
    virtual ~FrameBufferManagerGL() override = default;

    virtual bool init() override;
    virtual IFrameBuffer *createFrameBuffer(std::string&& name) override;
    virtual IFrameBuffer *frameBuffer(const std::string& name) const override;
    virtual IFrameBuffer *frameBuffer(uint32_t id) const override;

private:
    std::map<uint32_t, std::unique_ptr<FrameBufferGL>> _frameBuffers;
};
#pragma once

#include <string>
#include "rendering/IFrameBuffer.hpp"

class FrameBufferGL final : public IFrameBuffer {
public:
    ~FrameBufferGL() override;

    bool init(const char *name) override;
    void destroy() override;
    bool attachTexture(Texture *texture, Attachment attachment) override;
    void bind() override;
    void unbind() override;

private:
    unsigned int _handle;
    std::string _name;

    bool hasFramebufferError();
};
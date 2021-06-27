#pragma once

class Texture;

class IFrameBuffer {
public:
    enum class Attachment { Color, Depth, Stencil, DepthStencil };

    virtual ~IFrameBuffer() = default;

    virtual bool init(const char *name) = 0;
    virtual void destroy() = 0;
    virtual bool attachTexture(Texture *texture, Attachment attachment) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;
};
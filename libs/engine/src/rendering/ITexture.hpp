#pragma once

#include <EASTL/string.h>
#include <cstdint>

namespace Texture {
    enum class WrapMode { Repeat = 0, MirroredRepeat, ClampToEdge, ClampToBorder };
    enum class Format { RGBA = 0, RGB };

    struct Params {
        Format format = Format::RGBA;
        WrapMode wrapMode = WrapMode::Repeat;
    };
}

class ITexture {
public:
    virtual ~ITexture() = default;

    virtual unsigned int handle() const = 0;

    virtual bool init(uint32_t width, uint32_t height, const Texture::Params& params) = 0;
    virtual bool init(const eastl::string& filePath, const Texture::Params& params) = 0;
    virtual void bind() const = 0;
    virtual void resize(uint32_t width, uint32_t height) = 0;
};

class ITextureManager {
public:
    virtual ~ITextureManager() = default;

    virtual ITexture* createTexture(uint32_t width, uint32_t height, const Texture::Params& params) = 0;
    virtual ITexture* createTexture(const eastl::string& filePath, const Texture::Params& params) = 0;
    virtual void deleteTexture(ITexture* texture) = 0;
};
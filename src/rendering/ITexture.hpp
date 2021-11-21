#pragma once

#include <string>
#include <cstdint>

class ITexture {
public:
    enum class WrapMode { Repeat = 0, MirroredRepeat, ClampToEdge, ClampToBorder };
    enum class Format{ RGBA = 0, RGB };

    struct Params {
        Format format = Format::RGBA;
        WrapMode wrapMode = WrapMode::Repeat;
    };

    virtual ~ITexture() = default;

    virtual bool init(uint32_t width, uint32_t height, const Params& params) = 0;
    virtual bool init(const std::string& filePath, const Params& params) = 0;
    virtual void bind() const = 0;
    virtual unsigned int handle() const = 0;
};

class ITextureManager {
public:
    virtual ~ITextureManager() = default;

    virtual ITexture* createTexture(uint32_t width, uint32_t height, const ITexture::Params& params) = 0;
    virtual ITexture* createTexture(const std::string& filePath, const ITexture::Params& params) = 0;
    virtual void deleteTexture(ITexture* texture) = 0;
};
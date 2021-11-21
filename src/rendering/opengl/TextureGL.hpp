#pragma once

#include <map>
#include <memory>

#include "rendering/ITexture.hpp"

class TextureGL final : public ITexture {
public:
    virtual ~TextureGL() override;

    unsigned int handle() const override { return _handle; }

    bool init(uint32_t width, uint32_t height, const Texture::Params& params) override;
    bool init(const std::string& filePath, const Texture::Params& params) override;
    void bind() const override;

private:
    unsigned int _handle = 0;
};

class TextureManagerGL final : public ITextureManager {
public:
    virtual ITexture* createTexture(uint32_t width, uint32_t height, const Texture::Params& params) override;
    virtual ITexture* createTexture(const std::string& filePath, const Texture::Params& params) override;
    virtual void deleteTexture(ITexture* texture) override;

private:
    std::map<int32_t, std::unique_ptr<ITexture>> _textures;
};
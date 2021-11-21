#pragma once

#include <map>
#include <memory>

#include "rendering/ITexture.hpp"

class TextureGL final : public ITexture {
public:
    virtual ~TextureGL() override;

    bool init(uint32_t width, uint32_t height, const Params& params) override;
    bool init(const std::string& filePath, const Params& params) override;
    void bind() const override;
    unsigned int handle() const override { return _handle; }

private:
    unsigned int _handle = 0;
};

class TextureManagerGL final : public ITextureManager {
public:


    virtual ITexture* createTexture(uint32_t width, uint32_t height, const ITexture::Params& params) override;
    virtual ITexture* createTexture(const std::string& filePath, const ITexture::Params& params) override;
    virtual void deleteTexture(ITexture* texture) override;

private:
    std::map<int32_t, std::unique_ptr<ITexture>> _textures;
};
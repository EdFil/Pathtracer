#pragma once

#include "rendering/Texture.hpp"

class TextureGL final : public Texture {
public:
    ~TextureGL();

    bool init(const char* filePath, const Params& params) override;
    void bind() const override;
};
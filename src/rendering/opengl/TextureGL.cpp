#include "rendering/opengl/TextureGL.hpp"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Logger.hpp"
#include "file/FileManager.hpp"
#include "UtilsGL.hpp"

namespace {

    constexpr GLenum formatToGL(const Texture::Format& format) {
        switch (format) {
            case Texture::Format::RGBA:
                return GL_RGBA;
            case Texture::Format::RGB:
                return GL_RGB;
            default:
                LOG_ERROR("[TextureGL] Unhandled format %d", format);
                return GL_INVALID_ENUM;
        }
    }

}  // namespace

// ---------------------------------------
// -------------  TextureGL  -------------
// ---------------------------------------

TextureGL::~TextureGL() {
    if (_handle != 0) {
        glDeleteTextures(1, &_handle);
    }
}

bool TextureGL::init(uint32_t width, uint32_t height, const Texture::Params& params) {
    if (_handle == 0) {
        glGenTextures(1, &_handle);
    }
    
    glBindTexture(GL_TEXTURE_2D, _handle);
    glTexImage2D(GL_TEXTURE_2D, 0, formatToGL(params.format), width, height, 0, formatToGL(params.format), GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return !UtilsGL::CheckGLError("[TextureGL]");
}

bool TextureGL::init(const eastl::string& filePath, const Texture::Params& params) {
    char fullPath[512];
    FileManager::instance()->fullPathForFile(filePath.c_str(), fullPath, sizeof(fullPath));

    int width, height, components;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* decodedImageData = stbi_load(fullPath, &width, &height, &components, STBI_rgb_alpha);
    if (decodedImageData == nullptr) {
        LOG_ERROR("[TextureGL] Cannot open %s", filePath.c_str());
        return false;
    }

    if (_handle == 0) {
        glGenTextures(1, &_handle);
    }

    glBindTexture(GL_TEXTURE_2D, _handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, decodedImageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(decodedImageData);
    return true;
}

void TextureGL::bind() const {
    glBindTexture(GL_TEXTURE_2D, _handle);
}

void TextureGL::resize(uint32_t width, uint32_t height) {
    if (_handle == 0) {
        LOG_WARN("[TextureGL] Failed to resize. Invalid texture handle");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, _handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

// ----------------------------------------------
// -------------  TextureManagerGL  -------------
// ----------------------------------------------

ITexture* TextureManagerGL::createTexture(uint32_t width, uint32_t height, const Texture::Params& params) {
    eastl::unique_ptr<TextureGL> texture = eastl::make_unique<TextureGL>();
    if (texture == nullptr || !texture->init(width, height, params)) {
        LOG_ERROR("[TextureManagerGL] Failed to create %ux%u texture. Texture(%p)", width, height, texture.get());
        return nullptr;
    }

    const auto it = _textures.emplace(texture->handle(), eastl::move(texture));
    if (!it.second) {
        LOG_ERROR("[TextureManagerGL] Failed to insert %dx%d texture into map. Memory allocation failed?", width, height);
        return nullptr;
    }

    return it.first->second.get();
}

ITexture* TextureManagerGL::createTexture(const eastl::string& filePath, const Texture::Params& params) {
    eastl::unique_ptr<TextureGL> texture = eastl::make_unique<TextureGL>();
    if (texture == nullptr || !texture->init(filePath, params)) {
        LOG_ERROR("[TextureManagerGL] Failed to create %s texture. Texture(%p)", filePath.c_str(), texture.get());
        return nullptr;
    }

    const auto it = _textures.emplace(texture->handle(), eastl::move(texture));
    if (!it.second) {
        LOG_ERROR("[TextureManagerGL] Failed to insert %s texture into map. Memory allocation failed?", filePath.c_str());
        return nullptr;
    }

    return it.first->second.get();
}

void TextureManagerGL::deleteTexture(ITexture* texture) {
    if (texture == nullptr) {
        LOG_WARN("[TextureManagerGL] Trying to delete nullptr texture. Skipping...");
        return;
    }
    
    _textures.erase(texture->handle());
}

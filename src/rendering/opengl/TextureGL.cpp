#include "rendering/opengl/TextureGL.hpp"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Logger.hpp"
#include "file/FileManager.hpp"

TextureGL::~TextureGL() {
    if (_handle != 0) {
        glDeleteTextures(1, &_handle);
    }
}

bool TextureGL::init(const char* filePath, const Params& params) {
    char fullPath[512];
    FileManager::instance()->fullPathForFile(filePath, fullPath, sizeof(fullPath));

    std::vector<char> fileData = FileManager::instance()->loadFile(filePath);
    int width, height, components;

    stbi_set_flip_vertically_on_load(true); 
    stbi_uc* decodedImageData = stbi_load(fullPath, &width, &height, &components, STBI_rgb_alpha);
    if (decodedImageData == nullptr) {
        LOG_ERROR("[TextureGL] Cannot open %s", filePath);
        return false;
    }

    glGenTextures(1, &_handle);
    glBindTexture(GL_TEXTURE_2D, _handle);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    LOG("%d", glGetError());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, decodedImageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(decodedImageData);
    return true;
}

void TextureGL::bind() const {
    glBindTexture(GL_TEXTURE_2D, _handle);
}

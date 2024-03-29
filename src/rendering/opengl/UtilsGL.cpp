#include "rendering/opengl/UtilsGL.hpp"

#include <glad/glad.h>

#include "Logger.hpp"

bool UtilsGL::CheckGLError(const char* logPrefix) {
    bool returnValue = false;
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        switch (errorCode) {
            case GL_INVALID_ENUM:
                LOG_ERROR("%s INVALID_ENUM", logPrefix);
                returnValue = true;
                break;
            case GL_INVALID_VALUE:
                LOG_ERROR("%s INVALID_VALUE", logPrefix);
                returnValue = true;
                break;
            case GL_INVALID_OPERATION:
                LOG_ERROR("%s INVALID_OPERATION", logPrefix);
                returnValue = true;
                break;
            case GL_OUT_OF_MEMORY:
                LOG_ERROR("%s OUT_OF_MEMORY", logPrefix);
                returnValue = true;
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                LOG_ERROR("%s INVALID_FRAMEBUFFER_OPERATION", logPrefix);
                returnValue = true;
                break;
            default:
                LOG_ERROR("%s Unhandled GL error code %d", logPrefix, errorCode);
        }
    }
    return returnValue;
}
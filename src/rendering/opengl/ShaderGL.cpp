#include "rendering/OpenGL/ShaderGL.hpp"

#include "Logger.hpp"

namespace {
GLenum convertShaderType(Shader::Type type) {
    switch (type) {
        case Shader::Type::Vertex:
            return GL_VERTEX_SHADER;
        case Shader::Type::Fragment:
            return GL_FRAGMENT_SHADER;
        default:
            LOG_ERROR("Unsuported shader type %d", type);
            return GL_INVALID_ENUM;
    }
}
}  // namespace

ShaderGL::~ShaderGL() {
    if (_handle != 0) {
        glDeleteShader(_handle);
    }
}

bool ShaderGL::init(Type type, const char* source) {
    GLenum shaderType = convertShaderType(type);
    GLuint handle = glCreateShader(shaderType);
    if (handle == 0) {
        LOG_ERROR("[ShaderGL] Failed to create shader object: %d", glGetError());
        return false;
    }

    if (!compileShader(handle, source)) {
        return false;
    }

    _handle = handle;
    _type = type;
    return true;
}

bool ShaderGL::reload(const char* source) {
    if (_handle == 0) {
        LOG_ERROR("[ShaderGL] Cannot reload shader when handle is invalid. Init was not called or failed.");
        return false;
    }

    return compileShader(_handle, source);
}

bool ShaderGL::compileShader(unsigned int handle, const char* source) const {
    glShaderSource(handle, 1, &source, nullptr);
    glCompileShader(handle);

    GLint status = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    if (!status) {
        char infoLog[512];
        glGetShaderInfoLog(handle, sizeof(infoLog), NULL, infoLog);
        LOG_ERROR("[ShaderGL] Failed to compile shader:\n%s\n%s", infoLog, source);
        return false;
    }

    return true;
}
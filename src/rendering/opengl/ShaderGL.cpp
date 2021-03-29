
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <SDL_stdinc.h>
#include "Logger.hpp"

#include "rendering/OpenGL/ShaderGL.hpp"

namespace {
GLenum convertShaderType(Shader::Type type) {
    switch (type) {
        case Shader::Type::Vertex:
            return GL_VERTEX_SHADER;
        case Shader::Type::Fragment:
            return GL_FRAGMENT_SHADER;
        default:
            LOG_ERROR("Unsuported shader type %s", type);
            return GL_INVALID_ENUM;
    }
}
}  // namespace

ShaderGL::ShaderGL(Type type, const std::string& source) : Shader(type, source) {
    compileShader();
}

ShaderGL::~ShaderGL() {
    deleteShader();
}

void ShaderGL::compileShader() {
    GLenum shaderType = convertShaderType(_type);
    const GLchar* sourceData = _source.data();
    
    _id = glCreateShader(shaderType);
    if (_id == 0) {

        return;
    }

    glShaderSource(_id, 1, &sourceData, nullptr);
    glCompileShader(_id);

    GLint status = 0;
    glGetShaderiv(_id, GL_COMPILE_STATUS, &status);
    if (!status) {
        LOG_ERROR("Failed to compile shader:\n %s", _source.data());
        printGLError();
        deleteShader();
    }
}

void ShaderGL::deleteShader() {
    if (_id != 0) {
        glDeleteShader(_id);
        _id = 0;
    }
}

void ShaderGL::printGLError() const {
    GLint logLength = 0;
    glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &logLength);
    
    char* log = SDL_stack_alloc(char, logLength);
    glGetShaderInfoLog(_id, logLength, nullptr, log);
    LOG_ERROR("%s", log);
    SDL_stack_free(log);
}
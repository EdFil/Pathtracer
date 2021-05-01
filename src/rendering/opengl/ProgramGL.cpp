#include "rendering/opengl/ProgramGL.hpp"

#include <glad/glad.h>
#include <cassert>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Logger.hpp"
#include "rendering/RenderingDevice.hpp"
#include "rendering/Shader.hpp"

namespace {
UniformData::Type fromGLType(GLenum type) {
    switch (type) {
        case GL_FLOAT:
            return UniformData::Type::Float;
        case GL_FLOAT_MAT4:
            return UniformData::Type::Mat4;
        case GL_SAMPLER_2D:
            return UniformData::Type::Texture;
        default:
            assert(false);
            return UniformData::Type::Undefined;
    }
}
}  // namespace

ProgramGL::~ProgramGL() {
    if (_handle != 0) {
        glDeleteProgram(_handle);
    }
}

bool ProgramGL::init(const Shader& vertexShader, const Shader& fragmentShader) {
    assert(vertexShader.type() == Shader::Type::Vertex);
    assert(fragmentShader.type() == Shader::Type::Fragment);

    GLuint handle = glCreateProgram();
    if (handle == 0) {
        LOG_ERROR("[ProgramGL] Error creating program. glError[%d]", glGetError());
        return false;
    }

    glAttachShader(handle, vertexShader.handle());
    glAttachShader(handle, fragmentShader.handle());

    GLenum value = glGetError();
    while (value != GL_NO_ERROR) {
        LOG_ERROR("GL ERROR: %d", value);
        value = glGetError();
    };

    if (!linkProgram(handle)) {
        return false;
    }

    _handle = handle;
    _vertexShader = &vertexShader;
    _fragmentShader = &fragmentShader;

    return true;
}

bool ProgramGL::setShader(const Shader& shader) {
    if (_handle == 0) {
        LOG_ERROR("[ProgramGL] Cannot set shader when handle is invalid. Init was not called or failed.");
        return false;
    }

    glAttachShader(_handle, shader.handle());
    if (!linkProgram(_handle)) {
        return false;
    }

    if (shader.type() == Shader::Type::Vertex) {
        _vertexShader = &shader;
    } else {
        _fragmentShader = &shader;
    }
    return true;
}

void ProgramGL::bind() const {
    glUseProgram(_handle);
}

bool ProgramGL::linkProgram(unsigned int handle) {
    glLinkProgram(handle);

    GLint status = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        char infoLog[512];
        glGetProgramInfoLog(handle, sizeof(infoLog), NULL, infoLog);
        LOG_ERROR("[ProgramGL] Error linking program. %s", infoLog);
        return false;
    }

    // Get uniform info from program
    GLint uniformMaxLength = 128;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLength);
    GLuint numActiveUniforms = 0;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, (GLint*)&numActiveUniforms);

    // Cache active uniform names
    GLchar* uniformNameBuffer = (GLchar*)alloca(uniformMaxLength);
    GLint uniformVariableSize;
    for (GLuint i = 0; i < numActiveUniforms; ++i) {
        GLenum type = GL_ZERO;
        GLint size;
        glGetActiveUniform(handle, i, uniformMaxLength, nullptr, &size, &type, uniformNameBuffer);
        GLint location = glGetUniformLocation(handle, uniformNameBuffer);
        if (location != -1) {
            _activeUniforms[uniformNameBuffer] = UniformData{fromGLType(type), (unsigned int)location};
        }
    }

    return true;
}

void ProgramGL::setUniform(const char* uniformName, int value) {
    auto it = _activeUniforms.find(uniformName);
    if (it != _activeUniforms.cend()) {
        glUniform1i((GLint)it->second.handle, value);
    }
}

void ProgramGL::setUniform(const char* uniformName, float value) {
    auto it = _activeUniforms.find(uniformName);
    if (it != _activeUniforms.cend()) {
        glUniform1f((GLint)it->second.handle, value);
    }
}

void ProgramGL::setUniform(const char* uniformName, const glm::ivec2& value) {
    auto it = _activeUniforms.find(uniformName);
    if (it != _activeUniforms.cend()) {
        glUniform2i((GLint)it->second.handle, value.x, value.y);
    }
}

void ProgramGL::setUniform(const char* uniformName, const glm::vec2& value) {
    auto it = _activeUniforms.find(uniformName);
    if (it != _activeUniforms.cend()) {
        glUniform2f((GLint)it->second.handle, value.x, value.y);
    }
}

void ProgramGL::setUniform(const char* uniformName, const glm::vec3& value) {
    auto it = _activeUniforms.find(uniformName);
    if (it != _activeUniforms.cend()) {
        glUniform3f((GLint)it->second.handle, value.x, value.y, value.y);
    }
}

void ProgramGL::setUniform(const char* uniformName, const glm::vec4& value) {
    auto it = _activeUniforms.find(uniformName);
    if (it != _activeUniforms.cend()) {
        glUniform4f((GLint)it->second.handle, value.x, value.y, value.z, value.w);
    }
}

void ProgramGL::setUniform(const char* uniformName, const glm::mat3& value) {
    auto it = _activeUniforms.find(uniformName);
    if (it != _activeUniforms.cend()) {
        glUniformMatrix3fv((GLint)it->second.handle, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void ProgramGL::setUniform(const char* uniformName, const glm::mat4& value) {
    auto it = _activeUniforms.find(uniformName);
    if (it != _activeUniforms.cend()) {
        glUniformMatrix4fv((GLint)it->second.handle, 1, GL_FALSE, glm::value_ptr(value));
    }
}

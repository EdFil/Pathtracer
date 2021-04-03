#include "rendering/OpenGL/ProgramGL.hpp"

#include <glad/glad.h>
#include <cassert>
#include "Logger.hpp"
#include "rendering/RenderingDevice.hpp"
#include "rendering/Shader.hpp"

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

    return true;
}

void ProgramGL::deleteProgram() {

}
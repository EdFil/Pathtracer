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
#include "rendering/IRenderingDevice.hpp"
#include "ShaderGL.hpp"

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

// ---------------------------------------
// -------------  ProgramGL  -------------
// ---------------------------------------

ProgramGL::~ProgramGL() {
    if (_handle != 0) {
        glDeleteProgram(_handle);
    }
}

bool ProgramGL::init(const IShader& vertexShader, const IShader& fragmentShader) {
    assert(vertexShader.type() == Shader::Type::Vertex);
    assert(fragmentShader.type() == Shader::Type::Fragment);
    const ShaderGL& vertexShaderGL = (const ShaderGL&)vertexShader;
    const ShaderGL& fragmentShaderGL = (const ShaderGL&)fragmentShader;

    GLuint handle = glCreateProgram();
    if (handle == 0) {
        LOG_ERROR("[ProgramGL] Error creating program. glError[%d]", glGetError());
        return false;
    }

    glAttachShader(handle, vertexShaderGL.handle());
    glAttachShader(handle, fragmentShaderGL.handle());

    GLenum value = glGetError();
    while (value != GL_NO_ERROR) {
        LOG_ERROR("GL ERROR: %d", value);
        value = glGetError();
    };

    if (!linkProgram(handle)) {
        return false;
    }

    _handle = handle;
    _vertexShader = &vertexShaderGL;
    _fragmentShader = &fragmentShaderGL;

    return true;
}

bool ProgramGL::setShader(const IShader& shader) {
    const ShaderGL& shaderGL = (const ShaderGL&)shader;

    if (_handle == 0) {
        LOG_ERROR("[ProgramGL] Cannot set shader when handle is invalid. Init was not called or failed.");
        return false;
    }

    glAttachShader(_handle, shaderGL.handle());
    if (!linkProgram(_handle)) {
        return false;
    }

    if (shader.type() == Shader::Type::Vertex) {
        _vertexShader = &shaderGL;
    } else {
        _fragmentShader = &shaderGL;
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

    //    GLuint numUniformBlocks = 0;
    //    glGetProgramiv(handle, GL_ACTIVE_UNIFORM_BLOCKS, (GLint*)&numUniformBlocks);
    //    GLint uniformBlockMaxLength = 128;
    //
    //    GLchar* uniformBlockNameBuffer = (GLchar*)alloca(uniformBlockMaxLength);
    //    for (GLuint i = 0; i < numUniformBlocks; i++) {
    //        GLint nameSize;
    //        GLint index;
    //        glGetActiveUniformBlockiv(handle, i, GL_UNIFORM_BLOCK_INDEX, &index);
    //        index = glGetUniformBlockIndex(handle, uniformBlockNameBuffer);
    //        glGetActiveUniformBlockName(handle, i, uniformBlockMaxLength, &nameSize, uniformBlockNameBuffer);
    //        LOG("[ProgramGL] Uniform block %s at index %d", uniformNameBuffer, index);
    //    }
    //
    //    GLint size;
    //    glGetActiveUniformBlockiv(_handle, 0, GL_ACTIVE_UNIFORM_BLOCKS, &size);

    GLuint matricesBlockIndex = glGetUniformBlockIndex(handle, "Matrices");
    if (matricesBlockIndex != GL_INVALID_INDEX)
        glUniformBlockBinding(handle, matricesBlockIndex, 0);

    GLuint lightBlockIndex = glGetUniformBlockIndex(handle, "Light");
    if (lightBlockIndex != GL_INVALID_INDEX)
        glUniformBlockBinding(handle, lightBlockIndex, 1);

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

const IShader* ProgramGL::vertexShader() const {
    return _vertexShader;
}

const IShader* ProgramGL::fragmentShader() const {
    return _fragmentShader;
}

// ----------------------------------------------
// -------------  ProgramManagerGL  -------------
// ----------------------------------------------

bool ProgramManagerGL::init(const IShaderManager& shaderManager) {
    const ShaderManagerGL& shaderManagerGL = (const ShaderManagerGL&)shaderManager;

    bool allCreated = true;
    allCreated |= createProgram(shaderManagerGL, Program::k_position, Shader::Vertex::k_position, Shader::Fragment::k_position);
    allCreated |= createProgram(shaderManagerGL, Program::k_positionTexture, Shader::Vertex::k_positionTexture, Shader::Fragment::k_positionTexture);
    allCreated |=
        createProgram(shaderManagerGL, Program::k_positionNormalTexture, Shader::Vertex::k_positionNormalTexture, Shader::Fragment::k_positionNormalTexture);

    return allCreated;
}

IProgram* ProgramManagerGL::createProgram(const std::string& name, const IShader& vertexShader, const IShader& fragmentShader) {
    if (IProgram* cachedProgram = program(name)) {
        return cachedProgram;
    }

    std::unique_ptr<ProgramGL> program = std::make_unique<ProgramGL>();
    if (!program || !program->init(vertexShader, fragmentShader)) {
        LOG_ERROR("[ProgramManagerGL] Failed to create Progra,. Program(%p) Name(%s)", program.get(), name.c_str());
        return nullptr;
    }

    const auto it = _programs.insert({name, std::move(program)});
    if (!it.second) {
        LOG_ERROR("[ProgramManagerGL] Failed to insert program into map. Memory allocation failed?");
        return nullptr;
    }

    ProgramGL* createdProgram = it.first->second.get();
    LOG("[ProgramManagerGL] Create Program success. Program(%p) Name(%s)", createdProgram, name.c_str());
    return createdProgram;
}

IProgram* ProgramManagerGL::program(const std::string& name) const {
    const auto it = _programs.find(name);
    if (it != _programs.cend()) {
        return it->second.get();
    }

    return nullptr;
}

bool ProgramManagerGL::createProgram(const ShaderManagerGL& shaderManager, const char* programName, const char* vertexName, const char* fragmentName) {
    ShaderGL* vertexShader = (ShaderGL*)shaderManager.shader(vertexName);
    ShaderGL* fragmentShader = (ShaderGL*)shaderManager.shader(fragmentName);

    if (vertexShader == nullptr || fragmentShader == nullptr) {
        LOG_ERROR("[ProgramManagerGL] Could not find a shader instance for %s or %s", vertexShader, fragmentShader);
        return false;
    }

    return createProgram(programName, *vertexShader, *fragmentShader) != nullptr;
}
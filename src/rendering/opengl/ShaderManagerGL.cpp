#include "rendering/opengl/ShaderManagerGL.hpp"

#include <fstream>

#include "Logger.hpp"
#include "rendering/opengl/ShaderGL.hpp"

ShaderManagerGL::~ShaderManagerGL() {
    for (auto& keyValuePair : _shaders) {
        delete keyValuePair.second;
    }
}

bool ShaderManagerGL::init() {
    bool allCreated = createShader(Shaders::Vertex::k_position, Shader::Type::Vertex, "shaders/opengl/pos3.vert") != nullptr;
    allCreated |= createShader(Shaders::Fragment::k_position, Shader::Type::Fragment, "shaders/opengl/pos3.frag") != nullptr;
    allCreated |= createShader(Shaders::Vertex::k_positionTexture, Shader::Type::Vertex, "shaders/opengl/pos3uv2.vert") != nullptr;
    allCreated |= createShader(Shaders::Fragment::k_positionTexture, Shader::Type::Fragment, "shaders/opengl/pos3uv2.frag") != nullptr;
    allCreated |= createShader(Shaders::Vertex::k_positionNormalTexture, Shader::Type::Vertex, "shaders/opengl/pos3norm3uv2.vert") != nullptr;
    allCreated |= createShader(Shaders::Fragment::k_positionNormalTexture, Shader::Type::Fragment, "shaders/opengl/pos3norm3uv2.frag") != nullptr;

    return allCreated;
}

Shader* ShaderManagerGL::createShader(const std::string& name, Shader::Type type, const char* fileName) {
    auto it = _shaders.find(name);
    if (it != _shaders.end()) {
        return it->second;
    }

    ShaderGL* shader = new ShaderGL();
    if (!shader->init(type, fileName)) {
        delete shader;
        return nullptr;
    }

    LOG("[ShaderManagerGL] %s shader created with name %s", Shader::shaderTypeToString(type), name.c_str());
    _shaders[name] = shader;
    return shader;
}

Shader* ShaderManagerGL::shader(const std::string& name) const {
    auto it = _shaders.find(name);
    if (it != _shaders.cend()) {
        return it->second;
    }

    return nullptr;
}
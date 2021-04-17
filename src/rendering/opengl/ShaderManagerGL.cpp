#include "rendering/OpenGL/ShaderManagerGL.hpp"

#include <fstream>

#include "rendering/OpenGL/ShaderGL.hpp"

ShaderManagerGL::~ShaderManagerGL() {
    for (auto& keyValuePair : _shaders) {
        delete keyValuePair.second;
    }
}

bool ShaderManagerGL::init() {
    bool allCreated = createShader(Shaders::Vertex::k_positionColor, Shader::Type::Vertex, "shaders/opengl/pos3.vert") != nullptr;
    allCreated |= createShader(Shaders::Fragment::k_positionColor, Shader::Type::Fragment, "shaders/opengl/pos3.frag") != nullptr;

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
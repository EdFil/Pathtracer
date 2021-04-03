#include "rendering/opengl/ProgramManagerGL.hpp"

#include "rendering/opengl/ProgramGL.hpp"
#include "rendering/opengl/ShaderManagerGL.hpp"

ProgramManagerGL::~ProgramManagerGL() {
    for (auto& keyValuePair : _programs) {
        delete keyValuePair.second;
    }
}

bool ProgramManagerGL::init(const ShaderManager& shaderManager) {
    const ShaderManagerGL& shaderManagerGL = (const ShaderManagerGL&)shaderManager;
    
    bool allCreated = true;
    Shader* positionColorVertex = shaderManagerGL.shader(Shaders::Vertex::k_positionColor);
    Shader* positionColorFragment = shaderManagerGL.shader(Shaders::Fragment::k_positionColor);
    if (positionColorVertex != nullptr && positionColorFragment != nullptr) {
        allCreated |= createProgram(Programs::k_positionColor, *positionColorVertex, *positionColorFragment) != nullptr;
    } else {
        allCreated = false;
    }

    return allCreated;
}

Program* ProgramManagerGL::createProgram(const std::string& name, const Shader& vertexShader, const Shader& fragmentShader) {
    auto it = _programs.find(name);
    if (it != _programs.cend()) {
        return it->second;
    }

    ProgramGL* program = new ProgramGL();
    if (!program->init(vertexShader, fragmentShader)) {
        delete program;
        return nullptr;
    }

    _programs[name] = program;
    return program;
}

Program* ProgramManagerGL::program(const std::string& name) {
    auto it = _programs.find(name);
    if (it != _programs.cend()) {
        return it->second;
    }

    return nullptr;
}
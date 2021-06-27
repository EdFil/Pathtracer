#include "rendering/opengl/ProgramManagerGL.hpp"
#include <Logger.hpp>

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
    allCreated |= createProgram(shaderManagerGL, Program::k_position, Shaders::Vertex::k_position, Shaders::Fragment::k_position);
    allCreated |= createProgram(shaderManagerGL, Program::k_positionTexture, Shaders::Vertex::k_positionTexture, Shaders::Fragment::k_positionTexture);
    allCreated |= createProgram(shaderManagerGL, Program::k_positionNormalTexture, Shaders::Vertex::k_positionNormalTexture, Shaders::Fragment::k_positionNormalTexture);
    
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

    LOG("[ProgramManagerGL] Program with name \"%s\" was created.", name.c_str());
    _programs[name] = program;
    return program;
}

Program* ProgramManagerGL::program(const std::string& name) const {
    auto it = _programs.find(name);
    if (it != _programs.cend()) {
        return it->second;
    }

    return nullptr;
}

bool ProgramManagerGL::createProgram(const ShaderManagerGL& shaderManager, const char* programName, const char* vertexName, const char* fragmentName) {
    Shader* vertexShader = shaderManager.shader(vertexName);
    Shader* fragmentShader = shaderManager.shader(fragmentName);
    
    if (vertexShader == nullptr && fragmentShader == nullptr) {
        LOG_ERROR("[ProgramManagerGL] Could not find a shader instance for %s or %s", vertexShader, fragmentShader);
        return false;
    }

    return createProgram(programName, *vertexShader, *fragmentShader) != nullptr;
}
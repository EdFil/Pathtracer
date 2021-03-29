#include "rendering/ProgramManager.hpp"

ProgramManager::ProgramManager(Renderer& renderer) : _renderer(renderer) {
}

bool ProgramManager::init() {
    // Create base shaders
    bool wereAllInitied = true;
    wereAllInitied &= createProgram("", "") != nullptr;

    return wereAllInitied;
}

Program* ProgramManager::createProgram(const std::string& vertexShader, const std::string& fragmentShader) {
    return nullptr;
}

#pragma once

#include <string>

class Program;
class Shader;
class ShaderManager;

namespace Programs {
extern const char* k_positionColor;
}

class ProgramManager {
public:
    ~ProgramManager() = default;

    virtual bool init(const ShaderManager& shaderManager) = 0;
    virtual Program* createProgram(const std::string& name, const Shader& vertex, const Shader& fragment) = 0;
    virtual Program* program(const std::string& name) = 0;
};
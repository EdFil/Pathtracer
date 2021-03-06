#pragma once

#include <map>
#include "rendering/ProgramManager.hpp"

class Program;
class ProgramGL;
class ShaderManager;
class ShaderManagerGL;
class Shader;

class ProgramManagerGL : public ProgramManager {
public:
    ~ProgramManagerGL();

    bool init(const ShaderManager& shaderManager) override;
    Program* createProgram(const std::string& name, const Shader& vertexShader, const Shader& fragmentShader) override;
    Program* program(const std::string& name) const override;

private:
    std::map<std::string, ProgramGL*> _programs;

    bool createProgram(const ShaderManagerGL& shaderManager, const char* programName, const char* vertexName, const char* fragmentName);
};
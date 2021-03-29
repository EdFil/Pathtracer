#pragma once

#include "rendering/Program.hpp"

class ShaderGL;
class Device;

class ProgramGL : public Program {
public:
    ProgramGL(const std::string& vertexShader, const std::string& fragmentShader);
    ~ProgramGL();

private:
    uint32_t _id;
    ShaderGL* _vertexShader;
    ShaderGL* _fragmentShader;

    void createProgram(Device& device);
    void deleteProgram();
};
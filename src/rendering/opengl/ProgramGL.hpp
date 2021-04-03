#pragma once

#include "rendering/Program.hpp"

#include <glad/glad.h>

class ShaderGL;
class Device;

class ProgramGL : public Program {
public:
    ~ProgramGL();

    bool init(const Shader& vertexShader, const Shader& fragmentShader) override;
    bool setShader(const Shader& vertexShader) override;
    void bind() const override;

private:
    bool linkProgram(unsigned int handle);
    void deleteProgram();
};
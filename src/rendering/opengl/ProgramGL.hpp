#pragma once

#include "rendering/Program.hpp"

#include <map>
#include <string>

class ShaderGL;
class Device;

class ProgramGL final : public Program {
public:
    ~ProgramGL() override;

    bool init(const Shader& vertexShader, const Shader& fragmentShader) override;
    bool setShader(const Shader& vertexShader) override;
    void bind() const override;

    void setUniform(const char* uniformName, int value) override;
    void setUniform(const char* uniformName, float value) override;
    void setUniform(const char* uniformName, const glm::ivec2& value) override;
    void setUniform(const char* uniformName, const glm::vec2& value) override;
    void setUniform(const char* uniformName, const glm::vec3& value) override;
    void setUniform(const char* uniformName, const glm::vec4& value) override;
    void setUniform(const char* uniformName, const glm::mat3& value) override;
    void setUniform(const char* uniformName, const glm::mat4& value) override;

private:
    std::map<std::string, int> _uniformHandles;

    bool linkProgram(unsigned int handle);
};
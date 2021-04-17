#pragma once

#include "rendering/ShaderManager.hpp"

#include <map>
#include <string>

class ShaderGL;

class ShaderManagerGL final : public ShaderManager {
public:
    ~ShaderManagerGL();

    bool init() override;
    Shader* createShader(const std::string& name, Shader::Type type, const char* fileName) override;
    Shader* shader(const std::string& name) const override;

private:
    std::map<std::string, ShaderGL*> _shaders;
};
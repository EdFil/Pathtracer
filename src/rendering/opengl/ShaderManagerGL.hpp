#pragma once

#include "rendering/ShaderManager.hpp"

#include <vector>
#include <memory>

class ShaderGL;

class ShaderManagerGL final : public ShaderManager {
public:
    ~ShaderManagerGL();

    bool init() override;
    Shader* createShader(Shader::Type type, const std::string& source) override;

private:
    std::vector<std::unique_ptr<ShaderGL>> _instances;
};
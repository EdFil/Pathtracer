#pragma once

#include <vector>

#include "rendering/Shader.hpp"

class ShaderManager {
public:
    virtual ~ShaderManager() = default;
    
    virtual bool init() = 0;
    virtual Shader* createShader(Shader::Type type, const std::string& source) = 0;
};

#pragma once

#include "rendering/Shader.hpp"

#include <glad/glad.h>

class ShaderGL : public Shader {
public:
    ~ShaderGL();

    bool init(Type type, const char* source) override;
    bool reload(const char* source) override;

private:
    bool compileShader(unsigned int handle, const char* source) const;
};
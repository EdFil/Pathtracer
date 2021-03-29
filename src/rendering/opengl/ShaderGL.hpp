#pragma once

#include "rendering/Shader.hpp"

class ShaderGL : public Shader {
public:
    ShaderGL(Type type, const std::string& source);
    ~ShaderGL();

    uint32_t id() const { return _id; }

private:
    uint32_t _id;

    void compileShader();
    void deleteShader();
    void printGLError() const;
};
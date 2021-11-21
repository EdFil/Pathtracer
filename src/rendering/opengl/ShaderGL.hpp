#pragma once

#include "rendering/IShader.hpp"

#include <map>
#include <memory>
#include <glad/glad.h>


class ShaderGL final : public IShader {
public:
    ~ShaderGL();

    unsigned int handle() const override { return _handle; };
    Shader::Type type() const override { return _type; };

    bool init(Shader::Type type, const char* fileName) override;
    bool reload(const char* source) override;

private:
    unsigned int _handle = 0;
    Shader::Type _type;

    bool compileShader(unsigned int handle, const char* source) const;
};

class ShaderManagerGL final : public IShaderManager {
public:
    virtual bool init() override;
    virtual IShader* createShader(const std::string& name, Shader::Type type, const char* fileName) override;
    virtual IShader* shader(const std::string& name) const override;
private:
    std::map<std::string, std::unique_ptr<ShaderGL>> _shaders;
};

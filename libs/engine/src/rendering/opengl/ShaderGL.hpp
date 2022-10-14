#pragma once

#include "rendering/IShader.hpp"

#include <EASTL/map.h>
#include <EASTL/unique_ptr.h>
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
    Shader::Type _type = Shader::Type::Invalid;

    bool compileShader(unsigned int handle, const char* source) const;
};

class ShaderManagerGL final : public IShaderManager {
public:
    virtual bool init() override;
    virtual IShader* createShader(const eastl::string& name, Shader::Type type, const char* fileName) override;
    virtual IShader* shader(const eastl::string& name) const override;
private:
    eastl::map<eastl::string, eastl::unique_ptr<ShaderGL>> _shaders;
};

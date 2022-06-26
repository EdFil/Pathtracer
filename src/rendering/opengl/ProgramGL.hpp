#pragma once

#include "rendering/IProgram.hpp"

#include <EASTL/unique_ptr.h>

class IShaderManager;
class ShaderGL;
class Device;
class ShaderManagerGL;

class ProgramGL final : public IProgram {
public:
    ~ProgramGL() override;

    bool init(const IShader& vertexShader, const IShader& fragmentShader) override;
    bool setShader(const IShader& vertexShader) override;
    void bind() const override;

    void setUniform(const char* uniformName, int value) override;
    void setUniform(const char* uniformName, float value) override;
    void setUniform(const char* uniformName, const glm::ivec2& value) override;
    void setUniform(const char* uniformName, const glm::vec2& value) override;
    void setUniform(const char* uniformName, const glm::vec3& value) override;
    void setUniform(const char* uniformName, const glm::vec4& value) override;
    void setUniform(const char* uniformName, const glm::mat3& value) override;
    void setUniform(const char* uniformName, const glm::mat4& value) override;

    bool isValid() const override { return _handle != 0; }
    const eastl::map<eastl::string, UniformData>& activeUniforms() const override { return _activeUniforms; }
    unsigned int handle() const override { return _handle; }
    inline const IShader* vertexShader() const override;
    inline const IShader* fragmentShader() const override;

private:
    unsigned int _handle;
    eastl::map<eastl::string, UniformData> _activeUniforms;
    const ShaderGL* _vertexShader = nullptr;
    const ShaderGL* _fragmentShader = nullptr;

    bool linkProgram(unsigned int handle);
};

class ProgramManagerGL final : public IProgramManager {
public:
    bool init(const IShaderManager& shaderManager) override;
    IProgram* createProgram(const eastl::string& name, const IShader& vertex, const IShader& fragment) override;
    IProgram* program(const eastl::string& name) const override;

private:
    eastl::map<eastl::string, eastl::unique_ptr<ProgramGL>> _programs;

    bool createProgram(const ShaderManagerGL& shaderManager, const char* programName, const char* vertexName, const char* fragmentName);
};
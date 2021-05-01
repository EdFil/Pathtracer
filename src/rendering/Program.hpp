#pragma once

#include <glm/fwd.hpp>

class Shader;

class Program {
public:
    static const char* k_position;
    static const char* k_positionTexture;
    static const char* k_positionNormalTexture;

    virtual ~Program() = default;

    virtual bool init(const Shader& vertexShader, const Shader& fragmentShader) = 0;
    virtual bool setShader(const Shader& vertexShader) = 0;

    virtual void setUniform(const char* uniformName, int value) = 0;
    virtual void setUniform(const char* uniformName, float value) = 0;
    virtual void setUniform(const char* uniformName, const glm::ivec2& value) = 0;
    virtual void setUniform(const char* uniformName, const glm::vec2& value) = 0;
    virtual void setUniform(const char* uniformName, const glm::vec3& value) = 0;
    virtual void setUniform(const char* uniformName, const glm::vec4& value) = 0;
    virtual void setUniform(const char* uniformName, const glm::mat3& value) = 0;
    virtual void setUniform(const char* uniformName, const glm::mat4& value) = 0;

    virtual void bind() const = 0;

    unsigned int handle() const { return _handle; }
    const Shader* vertexShader() const { return _vertexShader; }
    const Shader* fragmentShader() const { return _fragmentShader; }

protected:
    unsigned int _handle;
    const Shader* _vertexShader = nullptr;
    const Shader* _fragmentShader = nullptr;
};
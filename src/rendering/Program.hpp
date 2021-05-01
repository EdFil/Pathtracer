#pragma once

#include <glm/fwd.hpp>
#include <map>
#include <string>

class Shader;

struct UniformData {
    enum class Type { Undefined, Float, Mat4, Texture };
    Type type;
    unsigned int handle;

    static unsigned int sizeInBytes(Type type) {
        switch (type) {
            case Type::Float:
                return sizeof(float);
            case Type::Texture:
                return sizeof(void*);
            case Type::Mat4:
                return sizeof(float) * 16;
            default:
                assert(false);
                return 4;
        }
    }
};

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

    bool isValid() const { return _handle != 0; }
    const std::map<std::string, UniformData>& activeUniforms() const { return _activeUniforms; }
    unsigned int handle() const { return _handle; }
    const Shader* vertexShader() const { return _vertexShader; }
    const Shader* fragmentShader() const { return _fragmentShader; }

protected:
    unsigned int _handle;
    std::map<std::string, UniformData> _activeUniforms;
    const Shader* _vertexShader = nullptr;
    const Shader* _fragmentShader = nullptr;
};
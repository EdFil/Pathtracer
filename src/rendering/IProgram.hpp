#pragma once

#include <string>
#include <map>
#include <glm/fwd.hpp>

class IShader;
class IShaderManager;

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

namespace Program {
    extern const char* k_position;
    extern const char* k_positionTexture;
    extern const char* k_positionNormalTexture;
}

class IProgram {
public:
    virtual ~IProgram() = default;

    virtual bool init(const IShader& vertexShader, const IShader& fragmentShader) = 0;
    virtual bool setShader(const IShader& vertexShader) = 0;

    virtual void setUniform(const char* uniformName, int value) = 0;
    virtual void setUniform(const char* uniformName, float value) = 0;
    virtual void setUniform(const char* uniformName, const glm::ivec2& value) = 0;
    virtual void setUniform(const char* uniformName, const glm::vec2& value) = 0;
    virtual void setUniform(const char* uniformName, const glm::vec3& value) = 0;
    virtual void setUniform(const char* uniformName, const glm::vec4& value) = 0;
    virtual void setUniform(const char* uniformName, const glm::mat3& value) = 0;
    virtual void setUniform(const char* uniformName, const glm::mat4& value) = 0;

    virtual void bind() const = 0;

    virtual bool isValid() const = 0;
    virtual const std::map<std::string, UniformData>& activeUniforms() const = 0;
    virtual unsigned int handle() const = 0;
    virtual const IShader* vertexShader() const = 0;
    virtual const IShader* fragmentShader() const = 0;
};

class IProgramManager {
public:
    virtual ~IProgramManager() = default;

    virtual bool init(const IShaderManager& shaderManager) = 0;
    virtual IProgram* createProgram(const std::string& name, const IShader& vertex, const IShader& fragment) = 0;
    virtual IProgram* program(const std::string& name) const = 0;
};
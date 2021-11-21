#pragma once

#include <string>

namespace Shader {
    enum class Type { Vertex, Fragment };

    namespace Vertex {
        extern const char* k_position;
        extern const char* k_positionTexture;
        extern const char* k_positionNormalTexture;
    }  // namespace Vertex

    namespace Fragment {
        extern const char* k_position;
        extern const char* k_positionTexture;
        extern const char* k_positionNormalTexture;
    }  // namespace Fragment

    const char* shaderTypeToString(Shader::Type type);
}  // namespace Shader

class IShader {
public:
    virtual ~IShader() = default;

    virtual unsigned int handle() const = 0;
    virtual Shader::Type type() const = 0;

    virtual bool init(Shader::Type type, const char* source) = 0;
    virtual bool reload(const char* source) = 0;
};

class IShaderManager {
public:
    virtual ~IShaderManager() = default;

    virtual bool init() = 0;
    virtual IShader* createShader(const std::string& name, Shader::Type type, const char* fileName) = 0;
    virtual IShader* shader(const std::string& name) const = 0;
};

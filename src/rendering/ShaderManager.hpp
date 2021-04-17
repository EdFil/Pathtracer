#pragma once

#include "rendering/Shader.hpp"
#include <string>

namespace Shaders {
namespace Vertex {
    extern const char* k_positionColor;
}
namespace Fragment {
    extern const char* k_positionColor;
}
}  // namespace Shaders

class ShaderManager {
public:
    virtual ~ShaderManager() = default;

    virtual bool init() = 0;
    virtual Shader* createShader(const std::string& name, Shader::Type type, const char* fileName) = 0;
    virtual Shader* shader(const std::string& name) const = 0;
};

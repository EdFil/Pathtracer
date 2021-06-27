#include "rendering/Shader.hpp"

const char* Shader::shaderTypeToString(Shader::Type type) {
    switch (type) {
        case Shader::Type::Vertex: return "Vertex";
        case Shader::Type::Fragment: return "Fragment";
    }
}
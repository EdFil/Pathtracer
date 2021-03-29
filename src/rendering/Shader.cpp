#include "rendering/Shader.hpp"

Shader::Shader(Type type, const std::string& source) : _type(type), _source(source) {
}

Shader::~Shader() = default;
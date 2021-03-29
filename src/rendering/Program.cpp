#include "rendering/Program.hpp"

Program::Program(const std::string& vertexShader, const std::string& fragmentShader)
  : _vertexShaderSource(vertexShader), _fragmentShaderSource(fragmentShader) {
}

Program::~Program() = default;
#include "rendering/IShader.hpp"

#include "Logger.hpp"

const char* Shader::Vertex::k_position = "positionColorVertex";
const char* Shader::Fragment::k_position = "positionColorFragment";
const char* Shader::Vertex::k_positionTexture = "positionTextureVertex";
const char* Shader::Fragment::k_positionTexture = "positionTextureFragment";
const char* Shader::Vertex::k_positionNormalTexture = "positionNormalTextureVertex";
const char* Shader::Fragment::k_positionNormalTexture = "positionNormalTextureFragment";

const char* Shader::shaderTypeToString(Shader::Type type) {
    switch (type) {
        case Shader::Type::Vertex:
            return "Vertex";
        case Shader::Type::Fragment:
            return "Fragment";
        default:
            LOG_ERROR("[Shader] Unhandled shader type %d", type);
            return "Error";
    }
}
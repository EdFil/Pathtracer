#include "rendering/OpenGL/ShaderManagerGL.hpp"

#include "rendering/OpenGL/ShaderGL.hpp"

ShaderManagerGL::~ShaderManagerGL() {
    for (auto& keyValuePair : _shaders) {
        delete keyValuePair.second;
    }
}

bool ShaderManagerGL::init() {
    const char* vertex =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* fragment =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    bool allCreated = createShader(Shaders::Vertex::k_positionColor, Shader::Type::Vertex, vertex) != nullptr;
    allCreated |= createShader(Shaders::Fragment::k_positionColor, Shader::Type::Fragment, fragment) != nullptr;

    return allCreated;
}

Shader* ShaderManagerGL::createShader(const std::string& name, Shader::Type type, const char* source) {
    auto it = _shaders.find(name);
    if (it != _shaders.end()) {
        return it->second;
    }

    ShaderGL* shader = new ShaderGL();
    if (!shader->init(type, source)) {
        delete shader;
        return nullptr;
    }

    _shaders[name] = shader;
    return shader;
}

Shader* ShaderManagerGL::shader(const std::string& name) const {
    auto it = _shaders.find(name);
    if (it != _shaders.cend()) {
        return it->second;
    }

    return nullptr;
}
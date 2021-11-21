#include "rendering/opengl/ShaderGL.hpp"

#include "Logger.hpp"
#include "file/FileManager.hpp"

namespace {
GLenum shaderTypeToGL(Shader::Type type) {
    switch (type) {
        case Shader::Type::Vertex:
            return GL_VERTEX_SHADER;
        case Shader::Type::Fragment:
            return GL_FRAGMENT_SHADER;
        default:
            LOG_ERROR("Unsuported shader type %d", type);
            return GL_INVALID_ENUM;
    }
}
}  // namespace

// --------------------------------------
// -------------  ShaderGL  -------------
// --------------------------------------

ShaderGL::~ShaderGL() {
    if (_handle != 0) {
        glDeleteShader(_handle);
    }
}

bool ShaderGL::init(Shader::Type type, const char* fileName) {
    GLenum shaderType = shaderTypeToGL(type);
    GLuint handle = glCreateShader(shaderType);
    if (handle == 0) {
        LOG_ERROR("[ShaderGL] Failed to create shader object: %d", glGetError());
        return false;
    }

    std::vector<char> fileData = FileManager::instance()->loadFile(fileName);
    if (fileData.empty() || !compileShader(handle, fileData.data())) {
        LOG_ERROR("[ShaderGL] Could not init shader \"%s\"", fileName);
        return false;
    }

    _handle = handle;
    _type = type;
    return true;
}

bool ShaderGL::reload(const char* source) {
    if (_handle == 0) {
        LOG_ERROR("[ShaderGL] Cannot reload shader when handle is invalid. Init was not called or failed.");
        return false;
    }

    return compileShader(_handle, source);
}

bool ShaderGL::compileShader(unsigned int handle, const char* source) const {
    glShaderSource(handle, 1, &source, nullptr);
    glCompileShader(handle);

    GLint status = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    if (!status) {
        char infoLog[512];
        glGetShaderInfoLog(handle, sizeof(infoLog), NULL, infoLog);
        LOG_ERROR("[ShaderGL] Failed to compile shader:\n%s\n%s", infoLog, source);
        return false;
    }

    return true;
}

// ---------------------------------------------
// -------------  ShaderManagerGL  -------------
// ---------------------------------------------

bool ShaderManagerGL::init() {
    bool allCreated = createShader(Shader::Vertex::k_position, Shader::Type::Vertex, "shaders/opengl/pos3.vert") != nullptr;
    allCreated |= createShader(Shader::Fragment::k_position, Shader::Type::Fragment, "shaders/opengl/pos3.frag") != nullptr;
    allCreated |= createShader(Shader::Vertex::k_positionTexture, Shader::Type::Vertex, "shaders/opengl/pos3uv2.vert") != nullptr;
    allCreated |= createShader(Shader::Fragment::k_positionTexture, Shader::Type::Fragment, "shaders/opengl/pos3uv2.frag") != nullptr;
    allCreated |= createShader(Shader::Vertex::k_positionNormalTexture, Shader::Type::Vertex, "shaders/opengl/pos3norm3uv2.vert") != nullptr;
    allCreated |= createShader(Shader::Fragment::k_positionNormalTexture, Shader::Type::Fragment, "shaders/opengl/pos3norm3uv2.frag") != nullptr;

    return allCreated;
}

IShader* ShaderManagerGL::createShader(const std::string& name, Shader::Type type, const char* fileName) {
    if (IShader* cachedShader = shader(name)) {
        return cachedShader;
    }

    std::unique_ptr<ShaderGL> shader = std::make_unique<ShaderGL>();
    if (!shader || !shader->init(type, fileName)) {
        LOG_ERROR("[ShaderManagerGL] Failed to create Shader. Shader(%p) Name(%s) Type(%s)", shader.get(), name.c_str(), Shader::shaderTypeToString(type));
        return nullptr;
    }
  
    const auto it = _shaders.insert({name, std::move(shader)});
    if (!it.second) {
        LOG_ERROR("[ShaderManagerGL] Failed to insert shader into map. Memory allocation failed?");
        return nullptr;
    }

    ShaderGL* createdShader = it.first->second.get();
    LOG("[ShaderManagerGL] Create Shader success. Shader(%p) Name(%s) Type(%s)", createdShader, name.c_str(), Shader::shaderTypeToString(type));
    return createdShader;
}

IShader* ShaderManagerGL::shader(const std::string& name) const {
    const auto it = _shaders.find(name);
    if (it != _shaders.cend()) {
        return it->second.get();
    }

    return nullptr;
}
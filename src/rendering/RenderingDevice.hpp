#pragma once

#include <string>

#include "rendering/Buffer.hpp"
#include "rendering/Shader.hpp"
#include "Texture.hpp"

class RenderingDeviceInfo;
class ShaderManager;
class Program;
class Texture;
class Camera;
class Mesh;

class RenderingDevice {
public:
    virtual ~RenderingDevice() = default;

    virtual bool init() = 0;
    virtual void preRender(Camera* camera) = 0;
    virtual void postRender() = 0;

    virtual Shader* createShader(const std::string& name, Shader::Type type, const char* source) = 0;
    virtual Program* getProgram(const std::string& name) const = 0;
    virtual Program* createProgram(const std::string& name, const Shader& vertexShader, const Shader& fragmentShader) = 0;
    virtual Buffer* createBuffer(const Buffer::Mode& mode) = 0;
    virtual Texture* createTexture(const char* filePath, const Texture::Params& params) = 0;

    virtual void render(Mesh* mesh, Program* program) = 0;

    virtual RenderingDeviceInfo* deviceInfo() = 0;
    virtual ShaderManager* shaderManager() = 0;
};
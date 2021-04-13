#pragma once

#include <string>

#include "rendering/Shader.hpp"
#include "rendering/Buffer.hpp"

class RenderingDeviceInfo;
class ShaderManager;
class Program;

class RenderingDevice {
public:
    virtual bool init() = 0;
    virtual void preRender() = 0;
    virtual void postRender() = 0;

    virtual Shader* createShader(const std::string& name, Shader::Type type, const char* source) = 0;
    virtual Program* createProgram(const std::string& name, const Shader& vertexShader, const Shader& fragmentShader) = 0;
	virtual Buffer* createBuffer(const Buffer::Params& params) = 0;

    virtual RenderingDeviceInfo* deviceInfo() = 0;
    virtual ShaderManager* shaderManager() = 0;
};
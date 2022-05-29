#pragma once

#include <EASTL/string.h>

#include "ITexture.hpp"
#include "rendering/IBuffer.hpp"
#include "rendering/IShader.hpp"

class IRenderingDeviceInfo;
class ShaderManager;
class Material;
class IProgram;
class ITexture;
class Camera;
class Mesh;
class IUniformBuffer;
class IFrameBufferManager;
class IShaderManager;
class IProgramManager;
class IUniformBufferManager;

class IRenderingDevice {
public:
    virtual ~IRenderingDevice() = default;

    virtual bool init() = 0;
    virtual void clearScreen(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f) = 0;
    virtual void preRender() = 0;
    virtual void postRender() = 0;

    virtual IBuffer* createBuffer(const Buffer::Mode& mode) = 0;

    virtual void render(Camera* camera, Mesh* mesh, Material* program) = 0;

    virtual IRenderingDeviceInfo* deviceInfo() = 0;
    virtual IShaderManager* shaderManager() = 0;
    virtual IFrameBufferManager* frameBufferManager() = 0;
    virtual ITextureManager* textureManager() = 0;
    virtual IProgramManager* programManager() = 0;
    virtual IUniformBufferManager* uniformBufferManager() = 0;
};
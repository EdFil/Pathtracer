#pragma once

#include <string>

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

class IRenderingDevice {
public:
    virtual ~IRenderingDevice() = default;

    virtual bool init() = 0;
    virtual void clearScreen() = 0;
    virtual void preRender(Camera* camera) = 0;
    virtual void postRender() = 0;

    virtual IUniformBuffer* createUniformBuffer(unsigned int bindingPoint, unsigned int sizeInBytes) = 0;
    virtual IBuffer* createBuffer(const Buffer::Mode& mode) = 0;

    virtual void render(Mesh* mesh, Material* program) = 0;

    virtual IRenderingDeviceInfo* deviceInfo() = 0;
    virtual IShaderManager* shaderManager() = 0;
    virtual IFrameBufferManager* frameBufferManager() = 0;
    virtual ITextureManager* textureManager() = 0;
    virtual IProgramManager* programManager() = 0;
};
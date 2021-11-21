#pragma once

#include "rendering/IRenderingDevice.hpp"

#include "BufferGL.hpp"
#include "ProgramGL.hpp"
#include "RenderingDeviceInfoGL.hpp"
#include "ShaderGL.hpp"
#include "FrameBufferGL.hpp"
#include "TextureGL.hpp"

struct SDL_Window;
struct SDL_Renderer;
typedef void* SDL_GLContext;

class RenderingDeviceGL : public IRenderingDevice {
public:
    RenderingDeviceGL(SDL_Window* window);
    ~RenderingDeviceGL();

    virtual bool init() override;
    virtual void clearScreen(float r, float g, float b, float a) override;
    virtual void preRender() override;
    virtual void postRender() override;

    virtual IUniformBuffer* createUniformBuffer(unsigned int bindingPoint, unsigned int sizeInBytes) override;
    virtual IBuffer* createBuffer(const Buffer::Mode& mode) override;

    virtual void render(Camera* camera, Mesh* mesh, Material* material) override;

    virtual IRenderingDeviceInfo* deviceInfo() override { return &_renderingDeviceInfo; }
    virtual IShaderManager* shaderManager() override { return &_shaderManager; }
    virtual IFrameBufferManager* frameBufferManager() override { return &_frameBufferManager; };
    virtual ITextureManager* textureManager() { return &_textureManager; };
    virtual IProgramManager* programManager() { return &_programManager; }

private:
    SDL_Window* _window = nullptr;
    SDL_GLContext _context = nullptr;
    RenderingDeviceInfoGL _renderingDeviceInfo;
    FrameBufferManagerGL _frameBufferManager;
    BufferManagerGL _bufferManager;
    TextureManagerGL _textureManager;
    ShaderManagerGL _shaderManager;
    ProgramManagerGL _programManager;
};
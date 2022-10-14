#pragma once

#include "rendering/IRenderingDevice.hpp"

#include "BufferGL.hpp"
#include "ProgramGL.hpp"
#include "RenderingDeviceInfoGL.hpp"
#include "ShaderGL.hpp"
#include "FrameBufferGL.hpp"
#include "TextureGL.hpp"
#include "UniformBufferGL.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_WindowEvent;
typedef void* SDL_GLContext;

class RenderingDeviceGL final : public IRenderingDevice {
public:
    RenderingDeviceGL(SDL_Window* window);
    ~RenderingDeviceGL();

    bool init() override;
    void clearScreen(float r, float g, float b, float a) override;
    void preRender() override;
    void postRender() override;
    void onSDLWindowEvent(const SDL_WindowEvent& event) override;
    virtual IBuffer* createBuffer(const Buffer::Mode& mode) override;

    virtual void render(Camera* camera, Mesh* mesh, Material* material) override;

    IRenderingDeviceInfo* deviceInfo() override { return &_renderingDeviceInfo; }
    IShaderManager* shaderManager() override { return &_shaderManager; }
    IFrameBufferManager* frameBufferManager() override { return &_frameBufferManager; };
    ITextureManager* textureManager() override { return &_textureManager; };
    IProgramManager* programManager() override { return &_programManager; }
    IUniformBufferManager* uniformBufferManager() override { return &_uniformBufferManager; }

private:
    SDL_Window* _window = nullptr;
    SDL_GLContext _context = nullptr;
    RenderingDeviceInfoGL _renderingDeviceInfo;
    FrameBufferManagerGL _frameBufferManager;
    BufferManagerGL _bufferManager;
    TextureManagerGL _textureManager;
    ShaderManagerGL _shaderManager;
    ProgramManagerGL _programManager;
    UniformBufferManagerGL _uniformBufferManager;
};
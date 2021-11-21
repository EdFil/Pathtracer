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
class Mesh;

class RenderingDeviceGL : public IRenderingDevice {
public:
    RenderingDeviceGL(SDL_Window* window);
    ~RenderingDeviceGL();

    bool init() override;
    void clearScreen() override;
    void preRender(Camera* camera) override;
    void postRender() override;

    IUniformBuffer* createUniformBuffer(unsigned int bindingPoint, unsigned int sizeInBytes) override;
    IBuffer* createBuffer(const Buffer::Mode& mode) override;

    void render(Mesh* mesh, Material* material) override;

    IRenderingDeviceInfo* deviceInfo() override { return &_renderingDeviceInfo; }
    IShaderManager* shaderManager() override { return &_shaderManager; }
    IFrameBufferManager* frameBufferManager() override { return &_frameBufferManager; };
    ITextureManager* textureManager() { return &_textureManager; };
    IProgramManager* programManager() { return &_programManager; }

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
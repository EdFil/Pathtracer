#pragma once

#include "rendering/RenderingDevice.hpp"

#include "BufferManagerGL.hpp"
#include "ProgramManagerGL.hpp"
#include "RenderingDeviceInfoGL.hpp"
#include "ShaderManagerGL.hpp"
#include "FrameBufferGL.hpp"
#include "TextureGL.hpp"

struct SDL_Window;
struct SDL_Renderer;
typedef void* SDL_GLContext;
class Mesh;

class RenderingDeviceGL : public RenderingDevice {
public:
    RenderingDeviceGL(SDL_Window* window);
    ~RenderingDeviceGL();

    bool init() override;
    void clearScreen() override;
    void preRender(Camera* camera) override;
    void postRender() override;

    Shader* createShader(const std::string& name, Shader::Type type, const char* source) override;
    Program* getProgram(const std::string& name) const override;
    Program* createProgram(const std::string& name, const Shader& vertexShader, const Shader& fragmentShader) override;
    IUniformBuffer* createUniformBuffer(unsigned int bindingPoint, unsigned int sizeInBytes) override;
    Buffer* createBuffer(const Buffer::Mode& mode) override;
    ITexture* createTexture(const char* filePath, const ITexture::Params& params) override;

    void render(Mesh* mesh, Material* material) override;

    RenderingDeviceInfo* deviceInfo() override { return &_renderingDeviceInfo; }
    ShaderManager* shaderManager() override { return &_shaderManager; }
    IFrameBufferManager* frameBufferManager() override { return &_frameBufferManager; };
    ITextureManager* textureManager() { return &_textureManager; };

private:
    SDL_Window* _window = nullptr;
    SDL_GLContext _context;
    RenderingDeviceInfoGL _renderingDeviceInfo;
    FrameBufferManagerGL _frameBufferManager;
    BufferManagerGL _bufferManager;
    TextureManagerGL _textureManager;
    ShaderManagerGL _shaderManager;
    ProgramManagerGL _programManager;
};
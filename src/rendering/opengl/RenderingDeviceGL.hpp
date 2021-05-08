#pragma once

#include "rendering/RenderingDevice.hpp"

#include "rendering/opengl/BufferManagerGL.hpp"
#include "rendering/opengl/ProgramManagerGL.hpp"
#include "rendering/opengl/RenderingDeviceInfoGL.hpp"
#include "rendering/opengl/ShaderManagerGL.hpp"

struct SDL_Window;
struct SDL_Renderer;
typedef void* SDL_GLContext;
class Mesh;

class RenderingDeviceGL : public RenderingDevice {
public:
    RenderingDeviceGL(SDL_Window* window);
    ~RenderingDeviceGL();

    bool init() override;
    void preRender(Camera* camera) override;
    void postRender() override;

    Shader* createShader(const std::string& name, Shader::Type type, const char* source) override;
    Program* getProgram(const std::string& name) const override;
    Program* createProgram(const std::string& name, const Shader& vertexShader, const Shader& fragmentShader) override;
    Buffer* createBuffer(const Buffer::Mode& mode) override;
    Texture* createTexture(const char* filePath, const Texture::Params& params) override;

    void render(Mesh* mesh, Material* material) override;

    RenderingDeviceInfo* deviceInfo() override { return &_renderingDeviceInfo; }
    ShaderManager* shaderManager() override { return &_shaderManager; }

private:
    SDL_Window* _window = nullptr;
    SDL_GLContext _context;
    BufferGL* _cameraBuffer = nullptr;
    RenderingDeviceInfoGL _renderingDeviceInfo;
    ShaderManagerGL _shaderManager;
    ProgramManagerGL _programManager;
    BufferManagerGL _bufferManager;
};
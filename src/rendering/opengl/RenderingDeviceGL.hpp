#pragma once

#include "rendering/RenderingDevice.hpp"

#include "rendering/OpenGL/RenderingDeviceInfoGL.hpp"
#include "rendering/OpenGL/ShaderManagerGL.hpp"

struct SDL_Window;
struct SDL_Renderer;
typedef void* SDL_GLContext;

class RenderingDeviceGL : public RenderingDevice {
public:
    RenderingDeviceGL(SDL_Window* window);
    ~RenderingDeviceGL();

    bool init() override;
    void preRender() override;
    void postRender() override;

    RenderingDeviceInfo* deviceInfo() override { return &_renderingDeviceInfo; }
    ShaderManager* shaderManager() override { return &_shaderManager; }

private:
    SDL_Window* _window;
    SDL_GLContext _context;
    RenderingDeviceInfoGL _renderingDeviceInfo;
    ShaderManagerGL _shaderManager;
};
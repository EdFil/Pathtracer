#pragma once

#include "rendering/RenderingDevice.hpp"

#include "rendering/OpenGL/RenderingDeviceInfoGL.hpp"
#include "rendering/OpenGL/ShaderManagerGL.hpp"
#include "rendering/OpenGL/ProgramManagerGL.hpp"

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

    Shader* createShader(const std::string& name, Shader::Type type, const char* source) override;
    Program* createProgram(const std::string& name, const Shader& vertexShader, const Shader& fragmentShader) override;
    
    RenderingDeviceInfo* deviceInfo() override { return &_renderingDeviceInfo; }
    ShaderManager* shaderManager() override { return &_shaderManager; }

private:
    SDL_Window* _window;
    SDL_GLContext _context;
    RenderingDeviceInfoGL _renderingDeviceInfo;
    ShaderManagerGL _shaderManager;
    ProgramManagerGL _programManager;
};
#include "rendering/OpenGL/RenderingDeviceGL.hpp"

#include <SDL_render.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>
#include <glad/glad.h>

#include "Logger.hpp"
#include "rendering/opengl/ProgramGL.hpp"
#include "rendering/opengl/RenderingDeviceInfoGL.hpp"
#include "rendering/opengl/ShaderGL.hpp"
#include "rendering/opengl/ShaderManagerGL.hpp"

#include "rendering/opengl/BufferGL.hpp"

#include "SDL_video.h"

Shader* shader = nullptr;
Buffer* buffer = nullptr;

RenderingDeviceGL::RenderingDeviceGL(SDL_Window* window) : _window(window), _context(nullptr) {
}

bool RenderingDeviceGL::init() {
    _context = SDL_GL_CreateContext(_window);
    if (_context == nullptr) {
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        LOG_ERROR("[RenderingDeviceGL] Could not initialize GLAD");
        return false;
    } else {
        LOG("[RenderingDeviceGL] OpenGL version %d.%d", GLVersion.major, GLVersion.minor);
    }

    bool wasSuccess = true;
    wasSuccess &= _renderingDeviceInfo.init();
    _renderingDeviceInfo.printInfo();
    wasSuccess &= _shaderManager.init();
    wasSuccess &= _programManager.init(_shaderManager);
    if (!wasSuccess) return false;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(_window, _context);
    ImGui_ImplOpenGL3_Init();

    int width, height;
    SDL_GetWindowSize(_window, &width, &height);
    glViewport(0, 0, width, height);

    float vertices[] = {
        0.0f,  0.5f,  0.0f,  // Vertex 1 (X, Y)
        0.5f,  -0.5f, 0.0f,  // Vertex 2 (X, Y)
        -0.5f, -0.5f, 0.0f   // Vertex 3 (X, Y)
    };

    Buffer::Params params;
    params.data = vertices;
    params.size = sizeof(vertices);
    buffer = _bufferManager.createBuffer(params);

    return wasSuccess;
}

RenderingDeviceGL::~RenderingDeviceGL() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(_context);
}

void RenderingDeviceGL::preRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(_window);
    ImGui::NewFrame();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingDeviceGL::postRender() {
    if (Program* program = _programManager.program(Programs::k_positionColor)) {
        program->bind();
    }
    glDrawArrays(GL_TRIANGLES, 0, 3);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(_window);
}

Shader* RenderingDeviceGL::createShader(const std::string& name, Shader::Type type, const char* source) {
    return _shaderManager.createShader(name, type, source);
}

Program* RenderingDeviceGL::createProgram(const std::string& name, const Shader& vertexShader, const Shader& fragmentShader) {
    return _programManager.createProgram(name, vertexShader, fragmentShader);
}

Buffer* RenderingDeviceGL::createBuffer(const Buffer::Params& params) {
    return _bufferManager.createBuffer(params);
}

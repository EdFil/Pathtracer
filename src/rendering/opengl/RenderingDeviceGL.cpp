#include "rendering/opengl/RenderingDeviceGL.hpp"

#include <SDL_render.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>
#include <glad/glad.h>

#include "Logger.hpp"
#include "rendering/opengl/BufferGL.hpp"
#include "rendering/opengl/ProgramGL.hpp"
#include "rendering/opengl/RenderingDeviceInfoGL.hpp"
#include "rendering/opengl/ShaderGL.hpp"
#include "rendering/opengl/ShaderManagerGL.hpp"
#include "rendering/opengl/TextureGL.hpp"

#include "base/Mesh.hpp"

#include "SDL_video.h"

RenderingDeviceGL::RenderingDeviceGL(SDL_Window* window) : _window(window) {
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
    if (!wasSuccess) {
        LOG_ERROR("Initial manager initalization failed");
        return false;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(_window, _context);
    ImGui_ImplOpenGL3_Init();

    _cameraBuffer = (BufferGL*)createBuffer(Buffer::Mode::UniformBlock);
    if (_cameraBuffer == nullptr) {
        LOG_ERROR("Could not create camera buffer");
        return false;
    }

    int width, height;
    SDL_GetWindowSize(_window, &width, &height);
    glViewport(0, 0, width, height);

    return true;
}

RenderingDeviceGL::~RenderingDeviceGL() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(_context);
}

void RenderingDeviceGL::preRender(Camera* camera) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(_window);
    ImGui::NewFrame();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingDeviceGL::postRender() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(_window);
}

Shader* RenderingDeviceGL::createShader(const std::string& name, Shader::Type type, const char* source) {
    return _shaderManager.createShader(name, type, source);
}

Program* RenderingDeviceGL::getProgram(const std::string& name) const {
    return _programManager.program(name);
}

Program* RenderingDeviceGL::createProgram(const std::string& name, const Shader& vertexShader, const Shader& fragmentShader) {
    return _programManager.createProgram(name, vertexShader, fragmentShader);
}

Buffer* RenderingDeviceGL::createBuffer(const Buffer::Mode& mode) {
    return _bufferManager.createBuffer(mode);
}

Texture* RenderingDeviceGL::createTexture(const char* filePath, const Texture::Params& params) {
    TextureGL* texture = new TextureGL();
    if (!texture->init(filePath, params)) {
        delete texture;
        return nullptr;
    }

    return texture;
}

void RenderingDeviceGL::render(Mesh* mesh, Program* program) {
    ((BufferGL*)mesh->buffer())->bind();
    ((ProgramGL*)program)->bind();

    glDrawArrays(GL_TRIANGLES, 0, mesh->count());
}

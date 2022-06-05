#include "rendering/opengl/RenderingDeviceGL.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <glad/glad.h>

#include "Logger.hpp"
#include "base/Material.hpp"
#include "base/Camera.hpp"
#include "rendering/opengl/BufferGL.hpp"
#include "rendering/opengl/ProgramGL.hpp"
#include "rendering/opengl/RenderingDeviceInfoGL.hpp"
#include "rendering/opengl/ShaderGL.hpp"
#include "rendering/opengl/TextureGL.hpp"
#include "rendering/opengl/UniformBufferGL.hpp"

#include "base/Mesh.hpp"

RenderingDeviceGL::RenderingDeviceGL(SDL_Window* window) : _window(window) {
}

RenderingDeviceGL::~RenderingDeviceGL() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(_context);
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

    int width, height;
    SDL_GetWindowSize(_window, &width, &height);
    glViewport(0, 0, width, height);

    SDL_GL_SetSwapInterval(0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    if (!_frameBufferManager.init()) {
        LOG_ERROR("[RenderingDeviceGL] Could not initialize FrameBufferManager");
        return false;
    }

    return true;
}

void RenderingDeviceGL::clearScreen(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderingDeviceGL::preRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(_window);
    ImGui::NewFrame();
}

void RenderingDeviceGL::postRender() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(_window);
}

IBuffer* RenderingDeviceGL::createBuffer(const Buffer::Mode& mode) {
    return _bufferManager.createBuffer(mode);
}

void RenderingDeviceGL::render(Camera* camera, Mesh* mesh, Material* material) {
    camera->bind();
    material->bind();
    ((BufferGL*)mesh->buffer())->bind();
    ((BufferGL*)mesh->buffer())->draw((unsigned int)mesh->count());
}

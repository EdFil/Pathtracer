#include "rendering/Renderer.hpp"

#include "Logger.hpp"
#include "rendering/Types.hpp"
#include "rendering/opengl/RenderingDeviceGL.hpp"
#include <SDL2/SDL_events.h>

Renderer::Renderer() = default;
Renderer::~Renderer() = default;

bool Renderer::init(SDL_Window* window, Camera* camera) {
    _sdlWindow = window;
    if (_sdlWindow == nullptr) {
        LOG_ERROR("[Renderer] Cannot initialize a renderer with an invalid window reference");
        return false;
    }

    _camera = camera;
    if (_camera == nullptr) {
        LOG_ERROR("[Renderer] Cannot initialize a renderer with an invalid camera reference");
        return false;
    }

    // FIXME: Don't force OpenGL like this when we have more
    setRenderingAPI(Renderer::API::OpenGL);

    return _renderingDevice.get() != nullptr;
}

Mesh* Renderer::createMesh(const Mesh::Params& params) {
    Mesh* mesh = new Mesh();
    if (!mesh->init(*_renderingDevice, params)) {
        delete mesh;
        return nullptr;
    }

    return mesh;
}

void Renderer::setRenderingAPI(Renderer::API renderingAPI) {
    switch (renderingAPI) {
        case Renderer::API::OpenGL:
            LOG("[Renderer] Initialising with an OpenGL rendering device");
            _renderingDevice = eastl::make_unique<RenderingDeviceGL>(_sdlWindow);
            break;
        default:
            break;
    }

    if (!_renderingDevice->init()) {
        _renderingDevice = nullptr;
    }
}

void Renderer::onSDLWindowEvent(const SDL_WindowEvent& event) {
    if (_renderingDevice != nullptr) {
        _renderingDevice->onSDLWindowEvent(event);
    }
}



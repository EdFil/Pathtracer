#include "rendering/Renderer.hpp"

#include "Logger.hpp"
#include "rendering/Types.hpp"
#include "rendering/opengl/RenderingDeviceGL.hpp"

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
    setRenderingAPI(rendering::API::OpenGL);

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

void Renderer::clearScreen() {
    _renderingDevice->clearScreen();
}

void Renderer::render(Mesh* mesh, Material* material) {
    _renderingDevice->render(mesh, material);
}

void Renderer::preRender() {
    _renderingDevice->preRender(_camera);
}

void Renderer::postRender() {
    _renderingDevice->postRender();
}

void Renderer::setRenderingAPI(rendering::API renderingAPI) {
    switch (renderingAPI) {
        case rendering::API::OpenGL:
            LOG("[Renderer] Initialising with an OpenGL rendering device");
            _renderingDevice = std::make_unique<RenderingDeviceGL>(_sdlWindow);
            break;
        default:
            break;
    }

    if (!_renderingDevice->init()) {
        _renderingDevice = nullptr;
    }
}
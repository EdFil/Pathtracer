#pragma once

#include <memory>

#include "rendering/Types.hpp"
#include "base/Mesh.hpp"

struct SDL_Window;
class RenderingDevice;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init(SDL_Window* window);

    Mesh* createMesh(const Mesh::Params& params);

    void preRender();
    void postRender();

    RenderingDevice* renderingDevice() const { return _renderingDevice.get(); }
    void setRenderingAPI(rendering::API renderingAPI);

private:
    SDL_Window* _sdlWindow;
    std::unique_ptr<RenderingDevice> _renderingDevice;
};

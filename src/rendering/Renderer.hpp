#pragma once

#include <memory>

#include "base/Mesh.hpp"
#include "rendering/Types.hpp"

struct SDL_Window;
class RenderingDevice;
class Camera;
class Material;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init(SDL_Window* window, Camera* _camera);

    Mesh* createMesh(const Mesh::Params& params);

    void clearScreen();
    void render(Mesh* mesh, Material* material);

    void preRender();
    void postRender();

    RenderingDevice* renderingDevice() const { return _renderingDevice.get(); }
    void setRenderingAPI(rendering::API renderingAPI);

private:
    Camera* _camera;
    SDL_Window* _sdlWindow;
    std::unique_ptr<RenderingDevice> _renderingDevice;
};

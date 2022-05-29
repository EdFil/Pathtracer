#pragma once

#include <EASTL/unique_ptr.h>

#include "base/Mesh.hpp"
#include "rendering/Types.hpp"

struct SDL_Window;
class IRenderingDevice;
class Camera;
class Material;

class Renderer {
public:
    enum class API { OpenGL };

    Renderer();
    ~Renderer();

    bool init(SDL_Window* window, Camera* _camera);

    Mesh* createMesh(const Mesh::Params& params);

    IRenderingDevice* renderingDevice() const { return _renderingDevice.get(); }
    void setRenderingAPI(Renderer::API renderingAPI);

private:
    Camera* _camera;
    SDL_Window* _sdlWindow;
    eastl::unique_ptr<IRenderingDevice> _renderingDevice;
};

#include <SDL.h>
#include <stdio.h>

#include <backends/imgui_impl_sdl.h>
#include <fast_obj.h>
#include <imgui.h>
#include <cstring>

#include "Color.hpp"
#include "Logger.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Window.hpp"
#include "base/Camera.hpp"
#include "base/Material.hpp"
#include "file/FileManager.hpp"
#include "rendering/Program.hpp"
#include "rendering/Renderer.hpp"
#include "rendering/RenderingDevice.hpp"
#include "rendering/Texture.hpp"

#include "glad/glad.h"

int main(int argc, char* argv[]) {
    Logger::init();
    FileManager::init();

    SDL_LogSetAllPriority(SDL_LogPriority::SDL_LOG_PRIORITY_VERBOSE);
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        LOG_ERROR("[SDL] Could not initialize! SDL_Error: %s", SDL_GetError());
        return -1;
    }

    Window window;
    Renderer renderer;
    Camera camera;

    bool isRunning = window.init() && renderer.init(window.window(), &camera);
    isRunning &= camera.init(*renderer.renderingDevice());

    char fullPath[256];
    FileManager::instance()->fullPathForFile("models/cube.obj", fullPath, sizeof(fullPath));
    fastObjMesh* suzanne = fast_obj_read(fullPath);

    struct Vertex {
        float px, py, pz;
        float tx, ty;
        float nx, ny, nz;
    };

#if 1
    std::vector<Vertex> vertexData(suzanne->face_count * 3);
    for (int i = 0; i < suzanne->face_count * 3; ++i) {
        Vertex vertex;

        memcpy(&vertex.px, suzanne->positions + suzanne->indices[i].p * 3, sizeof(float) * 3);
        memcpy(&vertex.tx, suzanne->texcoords + suzanne->indices[i].t * 2, sizeof(float) * 2);
        memcpy(&vertex.nx, suzanne->normals + suzanne->indices[i].n * 3, sizeof(float) * 3);

        vertexData.push_back(std::move(vertex));
    }

    Mesh::Params meshParams;
    meshParams.vertices = VectorView<float>(&vertexData[0].px, vertexData.size() * 8);
    Mesh* mesh = renderer.createMesh(meshParams);
#else
    std::vector<Vertex> vertexData(suzanne->position_count * 2);
#endif

    LOG("KB size = %f", (vertexData.size() * sizeof(vertexData[0]))  / 1024.0f);
    Texture* texture = renderer.renderingDevice()->createTexture("textures/sample.jpg", {});

    Material material;
    material.init(renderer.renderingDevice()->getProgram(Program::k_positionNormalTexture));
    material.setValue("modelMatrix", glm::mat4(1.0f));
    material.setTexture("Texture01", texture);

    Uint32 previousTime = SDL_GetTicks();
    Uint32 currentTime = previousTime;

    while (isRunning) {
        float deltaTime = (currentTime - previousTime) / 100.0f;

        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent) != 0) {
            ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

            if (sdlEvent.type == SDL_QUIT || (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_ESCAPE)) {
                isRunning = false;
            } else if (sdlEvent.type == SDL_WINDOWEVENT) {
                window.onSDLEvent(sdlEvent.window);
            } else if (sdlEvent.type == SDL_MOUSEMOTION) {
                // const SDL_MouseMotionEvent& motionEvent = sdlEvent.motion;
                // if ((motionEvent.state & SDL_BUTTON_LEFT) != 0) {
                //    Color color(rand() % 256, rand() % 256, rand() % 256, 255);
                //    renderer.drawPixel(color, motionEvent.x, motionEvent.y);
                //}
            }
        }

        camera.update(deltaTime);
        renderer.preRender();
        ImGui::ShowDemoWindow();

        renderer.renderingDevice()->render(mesh, &material);

        ImGui::Render();
        renderer.postRender();

        previousTime = currentTime;
        currentTime = SDL_GetTicks();
    }

    FileManager::destroy();
    Logger::destroy();

    return 0;
}

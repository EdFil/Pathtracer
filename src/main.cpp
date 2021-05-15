#include <SDL.h>
#include <stdio.h>

#include <backends/imgui_impl_sdl.h>
#include <fast_obj.h>
#include <imgui.h>
#include <chrono>
#include <cstring>

#include "Color.hpp"
#include "Logger.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Window.hpp"
#include "base/Camera.hpp"
#include "base/Light.hpp"
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
    Light light;

    bool isRunning = window.init() && renderer.init(window.window(), &camera);
    isRunning &= camera.init(*renderer.renderingDevice());
    isRunning &= light.init(*renderer.renderingDevice());

    std::chrono::system_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    char fullPath[256];
    FileManager::instance()->fullPathForFile("models/suzanne.obj", fullPath, sizeof(fullPath));
    fastObjMesh* suzanne = fast_obj_read(fullPath);

    std::chrono::system_clock::time_point timeAfterFileParsing = std::chrono::high_resolution_clock::now();

    struct Vertex {
        float px, py, pz;
        float tx, ty;
        float nx, ny, nz;

        bool operator==(const Vertex& rhs) const { return memcmp(this, &rhs, sizeof(Vertex)) == 0; }

        bool operator!=(const Vertex& rhs) const { return memcmp(this, &rhs, sizeof(Vertex)) != 0; }
    };

    std::vector<unsigned int> indices;
#if 0
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
    std::vector<bool> dataExists(suzanne->position_count, false);
    std::vector<Vertex> vertexData(suzanne->position_count);
    std::vector<Vertex> duplicatedVertices;
    duplicatedVertices.reserve(suzanne->position_count);
    indices.reserve(suzanne->face_count * 3);

    for (int i = 0; i < suzanne->face_count * 3; ++i) {
        const fastObjIndex& index = suzanne->indices[i];
        Vertex vertex;

        memcpy(&vertex.px, suzanne->positions + index.p * 3, sizeof(float) * 3);
        memcpy(&vertex.tx, suzanne->texcoords + index.t * 2, sizeof(float) * 2);
        memcpy(&vertex.nx, suzanne->normals + index.n * 3, sizeof(float) * 3);

        if (!dataExists[index.p]) {
            // New vertex
            vertexData[index.p] = vertex;
            dataExists[index.p] = true;
            indices.push_back(index.p - 1);
        } else if (vertexData[index.p] != vertex) {
            // We need to duplicate vertex
            auto it = std::find(duplicatedVertices.cbegin(), duplicatedVertices.cend(), vertex);
            if (it != duplicatedVertices.cend()) {
                // It was already duplicated, let's use it
                const int index = suzanne->position_count + it - duplicatedVertices.cbegin() - 1;
                indices.push_back(index);
            } else {
                // Let's create and add it to the duplicated list
                duplicatedVertices.push_back(vertex);
                const int index = suzanne->position_count + duplicatedVertices.size() - 2;
                indices.push_back(index);
            }
        } else {
            // Vertex already exists in the main vertex data
            indices.push_back(index.p - 1);
        }
    }

    vertexData.insert(vertexData.end(), duplicatedVertices.begin(), duplicatedVertices.end());

    Mesh::Params meshParams;
    meshParams.vertices = VectorView<float>(&vertexData[1].px, (vertexData.size() - 1) * 8);
    meshParams.indices = VectorView<unsigned int>(indices.data(), indices.size());
    Mesh* mesh = renderer.createMesh(meshParams);
#endif

    std::chrono::system_clock::time_point timeAfterMeshCreation = std::chrono::high_resolution_clock::now();

    LOG("Parsing time %d Mesh creation time %d", std::chrono::duration_cast<std::chrono::milliseconds>(timeAfterFileParsing - startTime).count(),
        std::chrono::duration_cast<std::chrono::milliseconds>(timeAfterMeshCreation - timeAfterFileParsing).count());

    LOG("Vertex KB size = %f Number %d", (vertexData.size() * sizeof(vertexData[0])) / 1024.0f, vertexData.size());
    LOG("Index KB size = %f Number %d", (indices.empty() ? 0.0f : (indices.size() * sizeof(indices[0])) / 1024.0f), indices.size());

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
        light.update(deltaTime);
        renderer.preRender();
        ImGui::ShowMetricsWindow();

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

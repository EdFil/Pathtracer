#include <SDL.h>
#include <stdio.h>

#include <backends/imgui_impl_sdl.h>
#include <imgui.h>
#include "Color.hpp"
#include "Logger.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Window.hpp"
#include "file/FileManager.hpp"
#include "rendering/Renderer.hpp"
#include "rendering/RenderingDevice.hpp"
#include "rendering/Program.hpp"

#include "glad/glad.h"

int main(int argc, char* argv[]) {
    Logger::init();
    FileManager::init();

    SDL_LogSetAllPriority(SDL_LogPriority::SDL_LOG_PRIORITY_VERBOSE);
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        LOG_ERROR("[SDL] Could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }

    Window window;
    Renderer renderer;
    Scene scene(renderer);
    Sphere sphere(Vec3f(0.0f, 0.0f, 0.0f), 1.0f);

    bool isRunning = window.init() && renderer.init(window.window()) && scene.init();

    std::vector<float> vertices{
        0.0f,  0.5f,  0.0f,  // Vertex 1 (X, Y)
        0.5f,  -0.5f, 0.0f,  // Vertex 2 (X, Y)
        -0.5f, -0.5f, 0.0f   // Vertex 3 (X, Y)
    };

    Mesh::Params meshParams;
    meshParams.vertices = VectorView<float>(vertices.data(), vertices.size());
    Mesh* mesh = renderer.createMesh(meshParams);

    while (isRunning) {
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent) != 0) {
            ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

            if (sdlEvent.type == SDL_QUIT) {
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

        renderer.preRender();
        ImGui::ShowDemoWindow();
        
        // <Hacky stuff to test the mesh class>
        if (Program* program = renderer.renderingDevice()->getProgram(Program::k_positionColor)) {
            program->bind();
        }
        glDrawArrays(GL_TRIANGLES, 0, 9);
        // </Hacky stuff to test the mesh class>

        scene.draw();
        ImGui::Render();
        renderer.postRender();
    }

    FileManager::destroy();
    Logger::destroy();

    return 0;
}

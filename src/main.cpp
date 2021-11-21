#include <SDL.h>
#include <stdio.h>

#include <backends/imgui_impl_sdl.h>
#include <fast_obj.h>
#include <imgui.h>
#include <chrono>
#include <cstring>

#include "Logger.hpp"
#include "Window.hpp"
#include "base/Camera.hpp"
#include "base/Light.hpp"
#include "base/Material.hpp"
#include "base/ResourceManager.hpp"
#include "file/FileManager.hpp"
#include "rendering/Program.hpp"
#include "rendering/Renderer.hpp"
#include "rendering/RenderingDevice.hpp"
#include "rendering/ITexture.hpp"
#include "rendering/IFrameBuffer.hpp"

bool enable1 = true;
bool enable2 = true;

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
    ResourceManager resourceManager;
    Camera camera;
    Light light;

    bool isRunning = window.init() && renderer.init(window.window(), &camera);
    isRunning &= camera.init(window, *renderer.renderingDevice());
    isRunning &= light.init(*renderer.renderingDevice());
    isRunning &= resourceManager.init(*renderer.renderingDevice());

    IFrameBufferManager* frameBufferManager = renderer.renderingDevice()->frameBufferManager();
    ITextureManager* textureManager = renderer.renderingDevice()->textureManager();

    IFrameBuffer* otherFrameBuffer = frameBufferManager->createFrameBuffer("Other");
    IFrameBuffer* defaultFrameBuffer = frameBufferManager->frameBuffer(0);
    if (!otherFrameBuffer || !defaultFrameBuffer) {
        LOG_ERROR("[main] Framebuffers are missing. Default(%p) Other(%p)", defaultFrameBuffer, otherFrameBuffer);
        return -1;
    }

    ITexture* texture = textureManager->createTexture(100, 100, ITexture::Params{});
    if (!texture || !otherFrameBuffer->attachTexture(texture, IFrameBuffer::Attachment::Color0)) {
        LOG_ERROR("[main] Framebuffer Color attachment is missing. Texture(%p)", texture);
        return -2;
    }

    Mesh* mesh = resourceManager.createMesh(Mesh::Primitive::Suzanne);
    ITexture* sampleTexture = renderer.renderingDevice()->createTexture("textures/sample.jpg", {});
    Material* material = resourceManager.createMaterial(Program::k_positionNormalTexture);
    material->setValue("modelMatrix", glm::mat4(1.0f));
    material->setTexture("Texture01", sampleTexture);

    Mesh* quad = resourceManager.createMesh(Mesh::Primitive::Plane);
    Material* quadMaterial = resourceManager.createMaterial(Program::k_positionTexture);
    quadMaterial->setValue("modelMatrix", glm::mat4(1.0f));
    quadMaterial->setTexture("Texture01", texture);

    Uint32 previousTime = SDL_GetTicks();
    Uint32 currentTime = previousTime;

    while (isRunning) {
        float deltaTime = (currentTime - previousTime) / 1000.0f;

        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent) != 0) {
            ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

            if (sdlEvent.type == SDL_QUIT || (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_ESCAPE)) {
                isRunning = false;
            } else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN && sdlEvent.button.button == 3) {
                window.setMouseGrab(!window.isMouseGrabbed());
            } else if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_r) {
                camera.reset();
            } else if (sdlEvent.type == SDL_WINDOWEVENT) {
                window.onSDLEvent(sdlEvent.window);
            }

            if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_1) {
                enable1 = !enable1;
            } else if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_2) {
                enable2 = !enable2;
            }
        }

        camera.update(deltaTime);
        light.update(deltaTime);
        
        renderer.preRender();
        {
            if (enable1) {
                otherFrameBuffer->bind();
                renderer.clearScreen();
                renderer.renderingDevice()->render(mesh, material);
            }

            if (true) {
                defaultFrameBuffer->bind();
                renderer.clearScreen();
                ImGui::ShowMetricsWindow();
                ImGui::Render();
                renderer.render(quad, quadMaterial);
            }
            
        }
        renderer.postRender();


        previousTime = currentTime;
        currentTime = SDL_GetTicks();
    }

    SDL_Quit();
    FileManager::destroy();
    Logger::destroy();

    return 0;
}

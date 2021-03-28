#include <SDL.h>
#include <stdio.h>

#include "Color.hpp"
#include "Logger.hpp"
#include "Renderer.hpp"
#include "Sphere.hpp"
#include "Window.hpp"

int main(int argc, char *argv[]) {
    Logger::init();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        LOG_ERROR("[SDL] Could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }

    Window window;
    Renderer renderer(window);
    Sphere sphere(Vec3f(0.0f, 0.0f, 0.0f), 1.0f);

    bool isRunning = window.init() && renderer.init();

    while (isRunning) {
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent) != 0) {
            if (sdlEvent.type == SDL_QUIT) {
                isRunning = false;
            } else if (sdlEvent.type == SDL_WINDOWEVENT) {
                window.onSDLEvent(sdlEvent.window);
            } else if (sdlEvent.type == SDL_MOUSEMOTION) {
                const SDL_MouseMotionEvent& motionEvent = sdlEvent.motion;
                if ((motionEvent.state & SDL_BUTTON_LEFT) != 0) {
                    Color color(rand() % 256, rand() % 256, rand() % 256, 255);
                    renderer.drawPixel(color, motionEvent.x, motionEvent.y);
                }
            }
        }

        renderer.preRender();
        renderer.postRender();
    }

    Logger::destroy();

    return 0;
}

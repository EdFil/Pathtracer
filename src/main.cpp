#include <SDL.h>
#include <stdio.h>

#include "Logger.hpp"
#include "Window.hpp"
#include "Renderer.hpp"

int main(int argc, char *argv[]) {
    Logger::init();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        LOG_ERROR("[SDL] Could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }

    Window window;
    Renderer renderer(window);
    
    bool isRunning = window.init() && renderer.init();

    while (isRunning) {
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent) != 0) {
            // User requests quit
            if (sdlEvent.type == SDL_QUIT) {
                isRunning = false;
            } else if (sdlEvent.type == SDL_WINDOWEVENT) {
                window.onSDLEvent(sdlEvent.window);
            }
        }

        renderer.preRender();
        renderer.postRender();
    }

    Logger::destroy();

    return 0;
}

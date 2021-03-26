#include "Window.hpp"

#include <SDL_events.h>
#include <SDL_video.h>
#include "Logger.hpp"
#include "SDLUtils.hpp"

Window::~Window() {
    if (_sdlWindow == nullptr) {
        LOG_WARN("[Window] Trying to destroy OSWindow when it's not created");
        return;
    }

    SDL_DestroyWindow(_sdlWindow);
}

bool Window::init(const WindowParams& params) {
    if (_sdlWindow != nullptr) {
        LOG_WARN("[Window] Already initialized. Skipping...");
        return true;
    }

    // Create OSWindow
    _sdlWindow = SDL_CreateWindow(params.name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, params.width,
                                  params.height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (_sdlWindow == nullptr) {
        LOG_ERROR("[SDL] Could not create a SDL OSWindow. %s", SDL_GetError());
        return false;
    }

    return true;
}

void Window::onSDLEvent(const SDL_WindowEvent& event) {
    SDL_WindowEventID windowEventID = (SDL_WindowEventID)event.event;
    LOG("[Window::onSDLEvent] %s", stringifyWindowEventID(windowEventID));

    switch (windowEventID) {
        case SDL_WINDOWEVENT_CLOSE:
            break;

        case SDL_WINDOWEVENT_RESIZED:
            break;

        case SDL_WINDOWEVENT_NONE:
        case SDL_WINDOWEVENT_SHOWN:
        case SDL_WINDOWEVENT_HIDDEN:
        case SDL_WINDOWEVENT_EXPOSED:
        case SDL_WINDOWEVENT_MOVED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
        case SDL_WINDOWEVENT_MINIMIZED:
        case SDL_WINDOWEVENT_MAXIMIZED:
        case SDL_WINDOWEVENT_RESTORED:
        case SDL_WINDOWEVENT_ENTER:
        case SDL_WINDOWEVENT_LEAVE:
        case SDL_WINDOWEVENT_FOCUS_GAINED:
        case SDL_WINDOWEVENT_FOCUS_LOST:
        case SDL_WINDOWEVENT_TAKE_FOCUS:
        case SDL_WINDOWEVENT_HIT_TEST:
            break;
    }
}


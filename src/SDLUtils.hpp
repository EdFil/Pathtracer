#pragma once

#include <SDL2/SDL_video.h>

const char* stringifyWindowEventID(SDL_WindowEventID eventID) {
    switch (eventID) {
        case SDL_WINDOWEVENT_NONE:
            return "SDL_WINDOWEVENT_NONE";
        case SDL_WINDOWEVENT_SHOWN:
            return "SDL_WINDOWEVENT_SHOWN";
        case SDL_WINDOWEVENT_HIDDEN:
            return "SDL_WINDOWEVENT_HIDDEN";
        case SDL_WINDOWEVENT_EXPOSED:
            return "SDL_WINDOWEVENT_EXPOSED";
        case SDL_WINDOWEVENT_MOVED:
            return "SDL_WINDOWEVENT_MOVED";
        case SDL_WINDOWEVENT_RESIZED:
            return "SDL_WINDOWEVENT_RESIZED";
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            return "SDL_WINDOWEVENT_SIZE_CHANGED";
        case SDL_WINDOWEVENT_MINIMIZED:
            return "SDL_WINDOWEVENT_MINIMIZED";
        case SDL_WINDOWEVENT_MAXIMIZED:
            return "SDL_WINDOWEVENT_MAXIMIZED";
        case SDL_WINDOWEVENT_RESTORED:
            return "SDL_WINDOWEVENT_RESTORED";
        case SDL_WINDOWEVENT_ENTER:
            return "SDL_WINDOWEVENT_ENTER";
        case SDL_WINDOWEVENT_LEAVE:
            return "SDL_WINDOWEVENT_LEAVE";
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            return "SDL_WINDOWEVENT_FOCUS_GAINED";
        case SDL_WINDOWEVENT_FOCUS_LOST:
            return "SDL_WINDOWEVENT_FOCUS_LOST";
        case SDL_WINDOWEVENT_CLOSE:
            return "SDL_WINDOWEVENT_CLOSE";
        case SDL_WINDOWEVENT_TAKE_FOCUS:
            return "SDL_WINDOWEVENT_TAKE_FOCUS";
        case SDL_WINDOWEVENT_HIT_TEST:
            return "SDL_WINDOWEVENT_HIT_TEST";
        default:
            return "ERROR";
    }
}
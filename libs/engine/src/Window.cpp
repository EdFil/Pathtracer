#include "Window.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include "Logger.hpp"
#include "SDLUtils.hpp"

// HACK!! MOVE THIS
void* operator new[](size_t size, const char* /*pName*/, int /*flags*/, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/)
{
    return ::new char[size];
}

// HACK!! MOVE THIS
void* operator new[](size_t size, size_t alignment, size_t /*alignmentOffset*/, const char* /*pName*/, int /*flags*/, unsigned /*debugFlags*/,
                     const char* /*file*/, int /*line*/)
{
    (void)alignment;
    // this allocator doesn't support alignment
    EASTL_ASSERT(alignment <= 8);
    return ::new char[size];
}

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
    uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
    _sdlWindow = SDL_CreateWindow(params.name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, params.width, params.height, flags);
    if (_sdlWindow == nullptr) {
        LOG_ERROR("[SDL] Could not create a SDL OSWindow. %s", SDL_GetError());
        return false;
    }

    LOG("[Window] Main window created. Handle(%d)", SDL_GetWindowID(_sdlWindow));
    return true;
}

void Window::setMouseGrab(bool shouldGrabMouse) {
    _shouldGrabMouse = shouldGrabMouse;
//    SDL_SetRelativeMouseMode((SDL_bool)shouldGrabMouse);
}

void Window::onSDLWindowEvent(const SDL_WindowEvent& windowEvent) {
    switch (windowEvent.event) {
        case SDL_WINDOWEVENT_RESIZED: {
            WindowEvent event(*this);
            event.data.size = size();
            _eventDispatcher.signal(WindowEventType::Resize, event);
            break;
        }

        case SDL_WINDOWEVENT_FOCUS_GAINED:
            setMouseGrab(true);
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            setMouseGrab(false);
            break;

        case SDL_WINDOWEVENT_CLOSE:
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
        case SDL_WINDOWEVENT_TAKE_FOCUS:
        case SDL_WINDOWEVENT_HIT_TEST:
        case SDL_WINDOWEVENT_ICCPROF_CHANGED:
        case SDL_WINDOWEVENT_DISPLAY_CHANGED:
            /* Ignore */
            break;
    }
}

glm::ivec2 Window::size() const {
    glm::ivec2 size;
    SDL_GetWindowSize(_sdlWindow, &size.x, &size.y);
    return size;
}

void Window::subscribe(WindowsEventObserver& observer) {
    _eventDispatcher.subscribe(observer);
}

void Window::unsubscribe(WindowsEventObserver& observer) {
    _eventDispatcher.unsubscribe(observer);
}
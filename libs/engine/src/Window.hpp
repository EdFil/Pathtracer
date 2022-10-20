#pragma once

#include <glm/vec2.hpp>

#include "EventDispatcher.hpp"

// HACK!! MOVE THIS
void* operator new[](size_t size, const char* /*pName*/, int /*flags*/, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/);
void* operator new[](size_t size, size_t alignment, size_t /*alignmentOffset*/, const char* /*pName*/, int /*flags*/, unsigned /*debugFlags*/,
                     const char* /*file*/, int /*line*/);

class Window;
struct SDL_Window;
struct SDL_WindowEvent;

struct WindowParams {
    const char name[32] = "Raytracer";
    const unsigned short width = 640;
    const unsigned short height = 480;
};

enum class WindowEventType { Resize };

union WindowEventData {
    void* __unused;
    glm::ivec2 size;
};

struct WindowEvent {
    WindowEvent(Window& window) : window(window) {}

    Window& window;
    WindowEventData data{0};
};

using WindowsEventObserver = EventDispatcher<WindowEventType, WindowEvent>::Observer;

class Window {
public:
    ~Window();

    bool init(const WindowParams& params = WindowParams());
    void onSDLWindowEvent(const SDL_WindowEvent& event);
    glm::ivec2 size() const;
    
    bool isMouseGrabbed() const { return _shouldGrabMouse; }
    void setMouseGrab(bool shouldGrabMouse);

    SDL_Window* window() const { return _sdlWindow; };
    void subscribe(WindowsEventObserver& observer);
    void unsubscribe(WindowsEventObserver& observer);

private:
    SDL_Window* _sdlWindow = nullptr;
    bool _shouldGrabMouse = false;
    EventDispatcher<WindowEventType, WindowEvent> _eventDispatcher;
};
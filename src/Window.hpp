#pragma once

#include "EventDispatcher.hpp"
#include "Vec2i.hpp"

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
    Vec2i size;
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
    void onSDLEvent(const SDL_WindowEvent& event);
    Vec2i size() const;

    SDL_Window* window() const { return _sdlWindow; };
    void subscribe(WindowsEventObserver& observer);
    void unsubscribe(WindowsEventObserver& observer);

private:
    SDL_Window* _sdlWindow = nullptr;
    EventDispatcher<WindowEventType, WindowEvent> _eventDispatcher;
};
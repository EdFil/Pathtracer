#pragma once

struct SDL_Window;
struct SDL_WindowEvent;

struct WindowParams {
    const char name[32] = "Raytracer";
    const unsigned short width = 640;
    const unsigned short height = 480;
};

class Window {
public:
    ~Window();

    bool init(const WindowParams& params = WindowParams());
    void onSDLEvent(const SDL_WindowEvent& event);

    SDL_Window* window() const { return _sdlWindow; };

private:
    SDL_Window* _sdlWindow = nullptr;
};
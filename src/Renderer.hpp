#pragma once

class Color;

class Window;
struct SDL_Renderer;
typedef void* SDL_GLContext;

struct RenderParams {};

class Renderer {
public:
    Renderer(Window& window);
    ~Renderer();

    bool init(RenderParams params = RenderParams());
    void preRender();
    void drawPixel(const Color& color, int x, int y);
    void postRender();
    void destroy();

private:
    Window& _window;
    SDL_Renderer* _renderer = nullptr;
    SDL_GLContext _context = nullptr;
};
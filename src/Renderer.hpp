#pragma once

#include "Window.hpp"

class Color;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
typedef void* SDL_GLContext;

struct RenderParams {};

class Renderer : public WindowsEventObserver {
public:
    Renderer(Window& window);
    ~Renderer();

    bool init(RenderParams params = RenderParams());
    void preRender();
    void drawPixel(const Color& color, int x, int y);
    void postRender();
    void destroy();

    Vec2i bufferSize() const;
    void onEventCalled(const WindowEventType& type, const WindowEvent& event) override;

private:
    Window& _window;
    SDL_Renderer* _renderer = nullptr;
    SDL_Surface* _surface = nullptr;
    SDL_Texture* _texture = nullptr;
    SDL_GLContext _context = nullptr;
};
#include "Renderer.hpp"

#include <SDL_render.h>
#include <glad/glad.h>
#include "Color.hpp"
#include "Logger.hpp"
#include "Window.hpp"

Renderer::Renderer(Window& window) : _window(window) {
}

Renderer::~Renderer() {
    destroy();
}

bool Renderer::init(RenderParams) {
    _renderer = SDL_CreateRenderer(_window.window(), -1, SDL_RENDERER_SOFTWARE);
    if (_renderer == nullptr) {
        LOG_ERROR("[Renderer] Could not create SDL renderer. %s", SDL_GetError());
        return false;
    }

    _context = SDL_GL_CreateContext(_window.window());
    if (_context == nullptr) {
        LOG_ERROR("[Renderer] Could not create OpenGL context. %s", SDL_GetError());
        return false;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        LOG_ERROR("[Renderer] Could not initialize GLAD");
        return false;
    }

    LOG("[RenderingDeviceGL] OpenGL version %d.%d", GLVersion.major, GLVersion.minor);
    return true;
}

void Renderer::preRender() {
    static float rCounter = 0;
    static float gCounter = 0;
    static float bCounter = 0;

    int value = rand() % 100;
    if (value < 33) {
        rCounter += 0.001f;
    } else if (value < 66) {
        gCounter += 0.001f;
    } else {
        bCounter += 0.001f;
    }

    short r = abs(sin(rCounter) * 255);
    short g = abs(sin(gCounter) * 255);
    short b = abs(sin(bCounter) * 255);

    SDL_SetRenderDrawColor(_renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);
}

void Renderer::postRender() {
    SDL_RenderPresent(_renderer);
}

void Renderer::drawPixel(const Color& color, int x, int y) {
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(_renderer, x, y);
}

void Renderer::destroy() {
    SDL_GL_DeleteContext(_context);
    SDL_DestroyRenderer(_renderer);

    _context = _renderer = nullptr;
}
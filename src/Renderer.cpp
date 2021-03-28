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
    
    Vec2i size = _window.size();
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, size.x, size.y);
    if (_texture == nullptr || SDL_LockTextureToSurface(_texture, nullptr, &_surface) != 0) {
        LOG_ERROR("[Renderer] Could not create surface texture. %s", SDL_GetError());
        return false;
    }

    _window.subscribe(*this);

    LOG("[RenderingDeviceGL] OpenGL version %d.%d", GLVersion.major, GLVersion.minor);
    return true;
}

void Renderer::preRender() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(_renderer);

    if (SDL_UpdateTexture(_texture, nullptr, _surface->pixels, _surface->pitch) != 0) {
        LOG_ERROR("Cannot update texture. %s", SDL_GetError());
    }
    SDL_RenderCopy(_renderer, _texture, nullptr, nullptr);
}

void Renderer::postRender() {
    
    SDL_RenderPresent(_renderer);
}

void Renderer::drawPixel(const Color& color, int x, int y) {
    uint32_t* pixels = (uint32_t*)_surface->pixels;
    pixels[(y * _surface->w) + x] = color.rgba;
}

void Renderer::destroy() {
    _window.unsubscribe(*this);

    SDL_DestroyTexture(_texture);
    SDL_GL_DeleteContext(_context);
    SDL_DestroyRenderer(_renderer);

    _context = _renderer = nullptr;
}

Vec2i Renderer::bufferSize() const {
    return _window.size();
}

void Renderer::onEventCalled(const WindowEventType& type, const WindowEvent& event) {
    if (event.window.window() != _window.window()) return;
    
    const WindowEventData& data = event.data;
    if (type == WindowEventType::Resize) {
        SDL_Surface* surface = nullptr;
        SDL_Texture* texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, data.size.x, data.size.y);
        if (texture == nullptr || SDL_LockTextureToSurface(texture, nullptr, &surface) != 0) {
            LOG_ERROR("[Renderer] Could not create surface texture. %s", SDL_GetError());
            SDL_DestroyTexture(texture);
        } else {
            SDL_DestroyTexture(_texture);
            _texture = texture;
            _surface = surface;
        }

    }
}

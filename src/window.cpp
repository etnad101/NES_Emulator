#include "window.h"
#include <iostream>
#include "SDL.h"

Window::Window(const char* name, int width, int height) {
    m_window = NULL;
    m_renderer = NULL;

    if ( SDL_INIT_VIDEO < 0) {
        std::cerr << "ERROR: SDL couldn't initialize (" << SDL_GetError() << ")\n";
        exit(-1);
    }

    m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    if ( m_window == NULL) {
        std::cerr << "ERROR: window could not be created (" << SDL_GetError() << ")\n";
        exit(-1);
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    if ( m_renderer == NULL) {
        std::cerr << "ERROR: renderer could not be created (" << SDL_GetError() << ")\n";
        exit(-1);
    }
}

void Window::clear() {
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_renderer);
}

void Window::drawPixel(int x, int y, int color) {
    SDL_SetRenderDrawColor(m_renderer, (color & 0xFF0000) >> 16, (color & 0x00FF00) >> 8, color & 0xFF, 255);
    SDL_RenderDrawPoint(m_renderer, x, y);
}


void Window::present() {
    SDL_RenderPresent(m_renderer);
}

void Window::cleanup() {
    SDL_DestroyWindow(m_window);
}

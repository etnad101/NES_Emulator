#pragma once
#include <iostream>
#include "SDL.h"
#include "iostream"

class Window {
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
public:
    Window(const char* name, int width, int height);
    void clear();
    void drawPixel(int x, int y, int color);
    void present();
    void cleanup();
};
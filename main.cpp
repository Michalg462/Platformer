#include <iostream>
#include <SDL.h>

#include "Stages/Stage.h"

#define DEFAULT_BG_COLOR_HEX 201e30
#define DEFAULT_BG_COLOR_RGB 32,30,48


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cout << "Failed to create window" << std::endl;
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr)
    {
        std::cout << "Failed to create renderer" << std::endl;
        return 1;
    }

    SDL_Surface *screen = SDL_GetWindowSurface(window);
    Uint32 default_bg_color = SDL_MapRGB(screen->format, DEFAULT_BG_COLOR_RGB);
    SDL_FillRect(screen, NULL, default_bg_color);

    Stage new_stage(256, 256);

    SDL_BlitSurface(new_stage.getScreen(), nullptr, screen, nullptr);
    SDL_UpdateWindowSurface(window);

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }
    return 0;
}
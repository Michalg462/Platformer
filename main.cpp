#include <iostream>
#include <SDL.h>

#include "Stages/Stage.h"
#include "Entities/Player/Player.h"

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

    Stage new_stage(512, 512);

    SDL_BlitSurface(new_stage.getScreen(), nullptr, screen, nullptr);
    SDL_UpdateWindowSurface(window);

    Player player(32,32,150, 100, {0,0});

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        SDL_FillRect(screen, NULL, default_bg_color);
        SDL_BlitSurface(new_stage.getScreen(), nullptr, screen, nullptr);

        player.update(new_stage.getTiles());
        SDL_Rect player_rect = {player.get_x(), player.get_y()};
        SDL_BlitSurface(player.get_sprite(), nullptr, screen, &player_rect);
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            player.controls(e);
        }
        SDL_UpdateWindowSurface(window);
    }
    return 0;
}
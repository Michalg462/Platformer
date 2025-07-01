#include <iostream>
#include <SDL.h>

#include "Stages/Stage.h"
#include "Entities/Player/Player.h"
#include "Entities/Box/Box.h"
#include "Stages/Menu/Menu.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600

#define DEFAULT_BG_COLOR_HEX 201e30
#define DEFAULT_BG_COLOR_RGB 32,30,48

#define OFFSET_X 240
#define OFFSET_Y 80

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
    SDL_FillRect(screen, nullptr, default_bg_color);

    Stage new_stage(480, 420);

    SDL_BlitSurface(new_stage.getScreen(), nullptr, screen, nullptr);
    SDL_UpdateWindowSurface(window);

    Player player(80,208,150, 100, {0,0});

    Box* box = new Box(172, 112, 3);
    Box* box2 = new Box(120, 112, 3);

    Menu menu = {288,192};
    Menu::write_text(menu.getScreen(), "A - move left  D - move right  SPACE - jump  ESC - Show menu",
        15, menu.getWidth()/2 - 7*8, 48, 16);

    std::vector<Entity*> entities;
    entities.push_back(&player);
    entities.push_back(box);
    entities.push_back(box2);

    SDL_Rect dst_rect;

    SDL_Event e;
    bool quit = false;
    bool show_menu = true;
    while (!quit)
    {
        SDL_FillRect(screen, nullptr, default_bg_color);
        dst_rect.x = OFFSET_X;
        dst_rect.y = OFFSET_Y;
        SDL_BlitSurface(new_stage.getScreen(), nullptr, screen, &dst_rect);

        for (int i = 0; i < entities.size(); i++)
        {
            entities[i]->update(new_stage.getTiles(), entities);
            dst_rect.x = entities[i]->get_x() + OFFSET_X;
            dst_rect.y = entities[i]->get_y() + OFFSET_Y;
            SDL_BlitSurface(entities[i]->get_sprite(), nullptr, screen, &dst_rect);
            if (!entities[i]->is_Alive())
            {
                delete entities[i];
                entities.erase(entities.begin() + i);
            }
        }

        if (show_menu)
        {
            dst_rect.x = screen->w/2 - menu.getWidth()/2;
            dst_rect.y = screen->h/2 - menu.getHeight()/2;
            SDL_BlitSurface(menu.getScreen(), nullptr, screen, &dst_rect);
        }

        Menu::write_text(screen, "Version: Proto 1.2", 20, screen->w - 20 * 8, screen->h - 16, 0);

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    show_menu == true ? show_menu = false : show_menu = true;
                }
            }
            player.controls(e);
        }
        SDL_UpdateWindowSurface(window);
    }
    return 0;
}
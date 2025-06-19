#include "Stage.h"
#include <cstdio>

#define BACKGROUND_TILE_SIZE 64
#define TERRAIN_TILE_SIZE 16
#define TERRAIN_SPREAD 13

#define BACKGROUND_PATH "src/backgrounds.bmp"
#define LVL_SHEET_PATH "src/terrain.bmp"
#define LVL_PATH "levels/lvl1/static.txt"

Stage::Stage(int width, int height) : width(width), height(height)
{
    screen = SDL_CreateRGBSurface(0, width, height, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!screen) {
        exit(-1);
    }

    int bg = 0;

    readStatic(LVL_PATH, &bg);

    buildStage(bg);
}

void Stage::readStatic(const std::string& path, int *bg)
{
    FILE *file = fopen(path.c_str(), "r");
    if (!file)
    {
        printf("Could not open file %s\n", path.c_str());
        exit(-1);
    }

    fscanf(file, " %d", bg);

    int x, y, type;
    std::vector<int> tmp;
    while (fscanf(file, "%d %d %d", &x, &y, &type) == 3)
    {
        tmp = {x, y, type};
        this->tiles.push_back(tmp);
    }

    fclose(file);
}

void Stage::buildStage(int bg)
{
    SDL_Surface* background_sheet = SDL_LoadBMP(BACKGROUND_PATH);
    if (!background_sheet) { SDL_Log("Failed to load BMP: %s", SDL_GetError()); }

    SDL_Rect src_rect = {bg*BACKGROUND_TILE_SIZE, 0,
        BACKGROUND_TILE_SIZE, BACKGROUND_TILE_SIZE};
    SDL_Rect dst_rect = {0, 0};

    for (int i = 0; i < height/BACKGROUND_TILE_SIZE; i++)
    {
        for (int j = 0; j < width/BACKGROUND_TILE_SIZE; j++)
        {
            dst_rect.x = j*BACKGROUND_TILE_SIZE;
            dst_rect.y = i*BACKGROUND_TILE_SIZE;
            SDL_BlitSurface(background_sheet, &src_rect, screen, &dst_rect);
        }
    }

    SDL_FreeSurface(background_sheet);

    SDL_Surface* lvl_sheet = SDL_LoadBMP(LVL_SHEET_PATH);
    if (!lvl_sheet) { SDL_Log("Failed to load LVL sheet: %s", SDL_GetError()); }
    for (int i = 0; i < this->tiles.size(); i++)
    {
        src_rect.x = (tiles[i][2] % TERRAIN_SPREAD) * TERRAIN_TILE_SIZE;
        src_rect.y = (tiles[i][2] / TERRAIN_SPREAD) * TERRAIN_TILE_SIZE;
        src_rect.w = TERRAIN_TILE_SIZE;
        src_rect.h = TERRAIN_TILE_SIZE;

        dst_rect.x = tiles[i][0];
        dst_rect.y = tiles[i][1];

        SDL_BlitSurface(lvl_sheet, &src_rect, screen, &dst_rect);
    }

}

int Stage::getHeight()
{
    return height;
}

int Stage::getWidth()
{
    return width;
}

SDL_Surface* Stage::getScreen()
{
    return screen;
}

Stage::~Stage()
{
    SDL_FreeSurface(screen);
}
#include "Stage.h"
#include <cstdio>
#include <iostream>
#include <ostream>

#define BACKGROUND_TILE_SIZE 64
#define TERRAIN_TILE_SIZE 16
#define TERRAIN_SPREAD 13

//#define BACKGROUND_PATH "src/backgrounds.bmp"
// #define LVL_SHEET_PATH "src/terrain.bmp"
// #define LVL_PATH "levels/lvl1/static.txt"



Stage::Stage(int width, int height) : width(width), height(height)
{
    /*
    *  Prepares the stage to be displayed in the screen.
    *  width - width of the stage canvas (pixels)
    *  height - height of the stage canvas (pixels)
    */
    screen = SDL_CreateRGBSurface(0, width, height, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!screen) {
        exit(-1);
    }

    // Will be used to prepare background of the level
    int bg = 0;

    readStatic(LVL_PATH, &bg);

    buildStage(bg);
}


void Stage::readStatic(const std::string& path, int *bg)
{
    /*
     *  Opens and hen reads configuration of a static parts of the stage (background, platforms, borders etc.).
     *  path - where the stage configuration file is located
     *  *bg - pointer to the background selector
     */
    std::cout << "readStatic: " << path << std::endl;

    FILE *file = fopen(path.c_str(), "r");
    if (!file)
    {
        printf("Could not open file %s\n", path.c_str());
        exit(-1);
    }

    fscanf(file, " %d", bg); // Reads the background

    int x, y, type;
    std::vector<int> tmp;
    while (fscanf(file, "%d %d %d", &x, &y, &type) == 3)
    {
        // Reads every part of the configuration till the end of file

        tmp = {x, y, type};
        this->tiles.push_back(tmp);
    }

    fclose(file);
}

void Stage::buildStage(int bg)
{
    /*
     *  Builds the graphical representation of the stage based on previously read configuration.
     *  bg - background chosen from the stage
     */

    // First loads the spreadsheet with backgrounds
    SDL_Surface* background_sheet = SDL_LoadBMP(BACKGROUND_PATH);
    if (!background_sheet) { SDL_Log("Failed to load BMP: %s", SDL_GetError()); }

    // Selects the chosen background, then blits it on the surface to build the whole background.
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
        dst_rect.x = (width/BACKGROUND_TILE_SIZE) * BACKGROUND_TILE_SIZE;
        std::cout << dst_rect.x << " " << dst_rect.y << std::endl;
        src_rect.w = width - (width/BACKGROUND_TILE_SIZE) * BACKGROUND_TILE_SIZE;
        std::cout << src_rect.w << std::endl;
        SDL_BlitSurface(background_sheet, &src_rect, screen, &dst_rect);
        src_rect.w = BACKGROUND_TILE_SIZE;
    }

    SDL_FreeSurface(background_sheet);


    // Loads the spreadsheet with all static images. Goes through list read in readStatic function, and blits given
    // blocks in their respective places. x and y are coordinates of the top-left corner, type is the number of the
    // sprite used, starting from top left moving right then down o the sheet
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
    SDL_FreeSurface(lvl_sheet);
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

std::vector<std::vector<int>> Stage::getTiles()
{
    return this->tiles;
}

Stage::~Stage()
{
    SDL_FreeSurface(screen);
}


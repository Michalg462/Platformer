#ifndef STAGE_H
#define STAGE_H

#include <string>
#include <vector>
#include <SDL.h>

class Stage {
protected:
    int width;
    int height;
    SDL_Surface* screen;
    std::vector<std::vector<int>> tiles;
    static constexpr auto BACKGROUND_PATH = "src/backgrounds.bmp";  // Path to a background bitmap
    static constexpr auto LVL_SHEET_PATH = "src/terrain.bmp";   // Path to bitmap that holds the terrain parts
    static constexpr auto LVL_PATH = "levels/lvl1/static.txt";  // Path to a file that holds the list of static elements of a level
    static constexpr auto TERRAIN_TILE_SIZE = 16;   // Size (in pixels) of a single terrain tile
public:
    Stage(int width, int height);

    void readStatic(const std::string& path, int *bg);

    void buildStage(int bg, const std::string& path, int tile_size);

    int getWidth();
    int getHeight();
    SDL_Surface* getScreen();
    std::vector<std::vector<int>> getTiles();

    ~Stage();
};


#endif //STAGE_H
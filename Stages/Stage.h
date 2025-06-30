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
    static constexpr auto BACKGROUND_PATH = "src/backgrounds.bmp";
    static constexpr auto LVL_SHEET_PATH = "src/terrain.bmp";
    static constexpr auto LVL_PATH = "levels/lvl1/static.txt";
public:
    Stage(int width, int height);

    void readStatic(const std::string& path, int *bg);

    void buildStage(int bg);

    int getWidth();
    int getHeight();
    SDL_Surface* getScreen();
    std::vector<std::vector<int>> getTiles();

    ~Stage();
};


#endif //STAGE_H
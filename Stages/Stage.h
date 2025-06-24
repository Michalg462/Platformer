#ifndef STAGE_H
#define STAGE_H

#include <string>
#include <vector>
#include <SDL.h>

class Stage {
    int width;
    int height;
    SDL_Surface* screen;
    std::vector<std::vector<int>> tiles;
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
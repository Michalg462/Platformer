#ifndef MENU_H
#define MENU_H
#include "../Stage.h"


class Menu : public Stage{
    static constexpr auto LVL_PATH = "levels/menu/menu.txt";

public:
    Menu(int width, int height);
    static void write_text(SDL_Surface* surface, const std::string& text, int max_line_length,
            int start_x, int start_y, int interline);

    ~Menu();
};



#endif //MENU_H

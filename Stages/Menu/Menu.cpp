#include "Menu.h"

#include <iostream>
#include <ostream>

#define TEXT_SPREAD_PATH "src/text_white.bmp"
#define TEXT_SPREAD_LEN 10
#define LETTER_WIDTH 8
#define LETTER_HEIGHT 10


Menu::Menu(int width, int height): Stage(width, height)
{
    /*
       *  Prepares the stage to be displayed in the screen.
       *  width - width of the stage canvas (pixels)
       *  height - height of the stage canvas (pixels)
       */
    int bg = 0;// Artifact from level, not used in menu
    readStatic(LVL_PATH, &bg);

    buildStage(bg, LVL_SHEET_PATH, TERRAIN_TILE_SIZE);
}

void Menu::write_text(SDL_Surface* surface, const std::string& text, int max_line_length,
                      int start_x, int start_y, int interline)
{
    /*
     *  Writes text on a given surface.
     *
     *  surface - SDL Surface on which the text will be written
     *  text - text to be written on a texture
     *  max_line_length - maximal length of a line of text
     *  start_x - X coordinate where the text will start (top-left corner)
     *  start_y - Y coordinate where the text will start (top-left coordinate)
     *  interline - space between the lines of thext (in pixels)
     */
    std::string upper_case_text = text;
    for (int i = 0; i < upper_case_text.length(); i++)
    {
        // Changing the whole text to uppercase
        upper_case_text[i] = toupper(upper_case_text[i]);
    }

    //  Loading the text bitmap from the file
    SDL_Surface *text_surface = SDL_LoadBMP(TEXT_SPREAD_PATH);
    if (text_surface == nullptr)
    {
        std::cout << "Unable to load image " << TEXT_SPREAD_PATH << std::endl;
    }

    // Preparing the rect that will be used to blit each letter on the surface
    SDL_Rect src_rect;
    src_rect.w = LETTER_WIDTH;
    src_rect.h = LETTER_HEIGHT;

    SDL_Rect dst_rect = {start_x, start_y}; // Preparing the destination rect, with first coordinates

    for (int i = 0; i < upper_case_text.length(); i++)
    {
        // loop used to "draw" all letter on the surface
        int ascii_code = upper_case_text[i];    // Getting ascii code of a current letter
        if (ascii_code >= 65 && ascii_code <= 90)
        {
            // If the letter is in the alphabet, it will get drawn here
            src_rect.x = ((ascii_code - 65) % TEXT_SPREAD_LEN) * LETTER_WIDTH;
            src_rect.y = ((ascii_code - 65) / TEXT_SPREAD_LEN) * LETTER_HEIGHT;
            SDL_BlitSurface(text_surface, &src_rect, surface, &dst_rect);
        }
        else if (ascii_code >= 48 && ascii_code <= 57)
        {
            // This one is for a numbers
            src_rect.x = (ascii_code - 48) * LETTER_WIDTH;
            src_rect.y = 30;
            SDL_BlitSurface(text_surface, &src_rect, surface, &dst_rect);
        }
        else
        {
            // All other cases of special characters
            src_rect.y = 40;
            switch (ascii_code)
            {
                case 46:
                    src_rect.x = 0;
                    break;
                case 44:
                    src_rect.x = 8;
                    break;
                case 58:
                    src_rect.x = 16;
                    break;
                case 63:
                    src_rect.x = 24;
                    break;
                case 33:
                    src_rect.x = 32;
                    break;
                case 40:
                    src_rect.x = 40;
                    break;
                case 41:
                    src_rect.x = 48;
                    break;
                case 43:
                    src_rect.x = 56;
                    break;
                case 45:
                    src_rect.x = 64;
                    break;
                default:
                    src_rect.x = 72;
                    break;
            }
            SDL_BlitSurface(text_surface, &src_rect, surface, &dst_rect);
        }
        dst_rect.x += LETTER_WIDTH; // Moving to the next free space to the right
        if (i % max_line_length == max_line_length - 1)
        {
            // If the text goes over the line length limit, goes to the next line
            dst_rect.y += LETTER_HEIGHT + interline;
            dst_rect.x = start_x;
        }
    }
}


Menu::~Menu(){}

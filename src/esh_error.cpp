#include "esh_error.h"
#include "game.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

void die(const char *error_message)
{
    SDL_Log("%s", error_message);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void panic(const char *error_message)
{
    SDL_Log("%s", error_message);
}


#include "SDL_integration.h"

#include <SDL_image.h>
#include <iostream>

extern SDL_Window *mainWindow;
extern SDL_Renderer *mainRenderer;

bool init()
{
    if (!SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cout << SDL_GetError() << '\n';
        return false;
    }
    if (!IMG_Init(IMG_INIT_PNG))
    {
        std::cout << IMG_GetError() << '\n';
        return false;
    }

    return true;
}

bool createWindow()
{
    mainWindow = SDL_CreateWindow("SDL_chess_v2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 640, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (mainWindow == NULL)
    {
        std::cout << SDL_GetError() << '\n';
        return false;
    }

    return true;
}

bool createRenderer()
{
    if (mainWindow == NULL)
        if (!createWindow())
            return false;

    mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (mainRenderer == NULL)
    {
        std::cout << SDL_GetError() << '\n';
        return false;
    }

    return true;
}

bool createWindowAndRenderer()
{
    if (!(createWindow() && createRenderer()))
        return false;

    return true;
}

SDL_Texture *loadTexture(std::string path)
{
    SDL_Surface *tempSurface = NULL;
    tempSurface = IMG_Load(path.c_str());
    if (tempSurface == NULL)
    {
        std::cout << IMG_GetError() << '\n';
        return NULL;
    }
    SDL_Texture *tempTexture = SDL_CreateTextureFromSurface(mainRenderer, tempSurface);
    if (tempTexture == NULL)
    {
        std::cout << SDL_GetError() << '\n';
        return NULL;
    }
    SDL_FreeSurface(tempSurface);

    return tempTexture;
}
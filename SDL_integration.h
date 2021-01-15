#ifndef SDL_INTEGRATION_H
#define SDL_INTEGRATION_H

#include <SDL.h>
#include <string>

//init SDL
bool init();

//create window
bool createWindow();

//create renderer
bool createRenderer();

//create window and renderer
bool createWindowAndRenderer();

SDL_Texture *loadTexture(std::string path);

#endif
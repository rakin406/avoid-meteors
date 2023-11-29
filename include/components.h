#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "SDL.h"

using Position = SDL_Point;
using Velocity = SDL_Point;

struct Texture
{
    SDL_Texture* texture;
};

#endif
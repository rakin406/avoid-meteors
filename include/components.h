#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "SDL.h"

using Color = SDL_Color;

struct Position
{
    int x {};
    int y {};
};

struct Velocity
{
    int x {};
    int y {};
};

struct Texture
{
    SDL_Texture* texture {};
};

/**
 * @brief Player idle texture.
 */
struct IdleTexture
{
    SDL_Texture* texture {};
};

/**
 * @brief Player running texture.
 */
struct RunningTexture
{
    SDL_Texture* texture {};
};

#endif
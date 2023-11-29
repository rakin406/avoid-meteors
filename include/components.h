#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "SDL.h"

struct Position
{
    float x {};
    float y {};
};

struct Velocity
{
    float x {};
    float y {};
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
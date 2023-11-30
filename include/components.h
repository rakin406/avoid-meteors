#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "SDL.h"


struct Position
{
    int x {};
    int y {};
};

struct Rotation
{
    double angle {};
    int x {};
    int y {};
};

struct Velocity
{
    int x {};
    int y {};
};

struct Transform
{
    Position position {};
};

struct Sprite
{
    // spritesheet, cutout in spritesheet
};

struct SpriteRenderer
{
}; // empty intentionally

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
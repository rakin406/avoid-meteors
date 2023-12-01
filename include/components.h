#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "SDL.h"

struct Rotation
{
    double angle {};
    SDL_Point* center {};
};

struct Transform
{
    SDL_Point position {};
    Rotation rotation {};
    SDL_Point scale {};
};

struct Sprite
{
    SDL_Texture* texture {};
};

struct SpriteAnimation
{
    SDL_Texture* texture {};
    SDL_Rect frameRec {};
    int currentFrame {};
    int framesCounter {};
    int framesSpeed {};
};

struct Velocity
{
    int x {};
    int y {};
};

#endif
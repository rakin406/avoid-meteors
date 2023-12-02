#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "SDL.h"

#include <vector>

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
    int animationFrames {};
    std::vector<SDL_Rect> spriteClips {};
};

struct Velocity
{
    int x {};
    int y {};
};

#endif
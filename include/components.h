#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "SDL.h"
#include <glm/glm.hpp>

struct Rotation
{
    double angle {};
    glm::ivec2 center {};
};

struct Transform
{
    glm::ivec2 position {};
    Rotation rotation {};
    glm::ivec2 scale {};
};

struct Sprite
{
    SDL_Texture* texture {};
};

struct SpriteAnimation
{
    SDL_Texture* texture {};
    glm::ivec2 position {};
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
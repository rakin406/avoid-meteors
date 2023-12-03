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

/**
 * @brief Information about an entity’s visual representation.
*/
struct Sprite
{
    SDL_Texture* texture {};
    SDL_Color* color {};
};

/**
 * @brief Represents an entity’s movement speed and direction.
*/
struct Velocity
{
    int x {};
    int y {};
};

/**
 * @brief Manages animations.
*/
struct Animation
{
    int totalFrames {};
    int currentFrame {};
    SDL_Rect* currentClip {};
    std::vector<SDL_Rect> spriteClips {};
};

#endif
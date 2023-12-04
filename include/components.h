#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "SDL.h"

/**
 * @brief Contains position, rotation, and scale information for an entity.
 */
struct Transform
{
    SDL_Point position {}; // 2D position (x, y)
    double rotation {};    // Rotation angle in degrees
    SDL_Point scale {};    // Scale factors (x, y)
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
    SDL_Rect frameRec {};
    SDL_Point frameSize {};
    int frameDuration {}; //  NOTE: Unit is milliseconds
};

#endif
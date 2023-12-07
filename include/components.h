/**
 * @file
 * @brief Provides components for entities.
 */

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "SDL.h"

/**
 * @brief Manages animations.
 */
struct Animation
{
    SDL_Rect frameRec {};
    SDL_RendererFlip flip {};
    int frameDuration {};
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
 * @brief Contains position, rotation, and scale information for an entity.
 */
struct Transform
{
    SDL_FPoint position {}; // 2D position (x, y)
    double rotation {};     // Rotation angle in degrees
    SDL_FPoint scale {};    // Scale factors (x, y)
};

/**
 * @brief Represents an entity’s movement speed and direction.
 */
struct Velocity
{
    float x {};
    float y {};
};

#endif

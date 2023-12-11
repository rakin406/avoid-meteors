/**
 * @file
 * @brief Provides global components for entities.
 */

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "SDL.h"

#include <string_view>

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
 * @brief Stores information about the current level.
 */
struct Level
{
    int levelNumber {};
    int numMeteors {};
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
 * @brief Contains information about the text to be displayed on the
 * screen.
 */
struct Text
{
    std::string_view content {}; // The actual text to be displayed
    int size {};                 // The size of the font
    SDL_Texture* texture {};     // The TEXTure
    SDL_Color color {};          // The color of the text
    SDL_FPoint position {};      // The position of the text
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

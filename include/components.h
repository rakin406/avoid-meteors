/**
 * @file
 * @brief Provides global components for entities.
 */

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "SDL.h"
#include <glm/glm.hpp>

#include <string_view>

using Velocity = glm::vec2;

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
    glm::vec2 position {};       // The position of the text
};

/**
 * @brief Contains position, rotation, and scale information for an entity.
 */
struct Transform
{
    glm::vec2 position {}; // 2D position (x, y)
    double rotation {};    // Rotation angle in degrees
    glm::vec2 scale {};    // Scale factors (x, y)
};

/**
 * @brief Direction state.
 */
// enum class Direction
//{
//     Left,
//     Right
// };

#endif

#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "components.h"

#include "SDL.h"

/**
 * @brief Class for operating on spritesheets.
 */
class AnimationSystem
{
public:
    /**
     * @brief Initialize the animation system.
     * @param texture Texture of the sprite image.
     * @param pos Position of the texture.
    */
    AnimationSystem(SDL_Texture* texture, const Position& pos);

private:
    SDL_Texture* texture {nullptr};
    SDL_Rect currentFrame {};
    Position pos {};
};

#endif
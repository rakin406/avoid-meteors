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
     * @param position Position of the texture.
     */
    AnimationSystem(SDL_Texture* texture, const Position& position);

private:
    SDL_Texture* texture {};
    SDL_Rect currentFrame {};
    Position position {};
};

#endif
#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

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
    AnimationSystem(SDL_Texture* texture, const SDL_Point& position);

private:
    SDL_Texture* texture {};
    SDL_Point position {};
    SDL_Rect frameRec {};
    int currentFrame {};
    int framesCounter {};
    int framesSpeed {}; // Number of spritesheet frames shown by second
};

#endif
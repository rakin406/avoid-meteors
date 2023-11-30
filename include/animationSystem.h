#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "renderWindow.h"

#include "SDL.h"

/**
 * @brief Class for operating on spritesheets.
 */
class AnimationSystem
{
public:
    /**
     * @brief Initialize the animation system.
     * @param window RenderWindow instance.
     * @param texture Texture of the sprite image.
     * @param position Position of the texture.
     * @param framesSpeed Number of spritesheet frames shown by second.
     */
    AnimationSystem(RenderWindow& window, SDL_Texture* texture,
                    const SDL_Point& position, int framesSpeed);

    /**
     * @brief Update frame.
     */
    void update();

private:
    RenderWindow* window {};
    SDL_Texture* texture {};
    SDL_Point position {};
    SDL_Rect frameRec {};
    int currentFrame {};
    int framesCounter {};
    int framesSpeed {};
};

#endif
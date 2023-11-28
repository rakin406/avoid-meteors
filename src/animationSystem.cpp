#include "animationSystem.h"
#include "components.h"

#include "SDL.h"

AnimationSystem::AnimationSystem(SDL_Texture* texture, const Position& pos)
    : texture { texture }, pos { pos }
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 32;
    currentFrame.h = 32;
    SDL_QueryTexture(texture, nullptr, nullptr, &currentFrame.w,
                     &currentFrame.h);
}
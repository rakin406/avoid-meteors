#include "animationSystem.h"

#include "SDL.h"

AnimationSystem::AnimationSystem(SDL_Texture* texture,
                                 const SDL_Point& position, int framesSpeed)
    : texture { texture }, position { position }, currentFrame { 0 },
      framesCounter { 0 }, framesSpeed { framesSpeed }
{
}
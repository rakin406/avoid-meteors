#include "animationSystem.h"
#include "colors.h"
#include "tools.h"

#include "SDL.h"

AnimationSystem::AnimationSystem(RenderWindow& window, SDL_Texture* texture,
                                 const SDL_Point& position, int framesSpeed)
    : window { &window }, texture { texture }, position { position },
      currentFrame { 0 }, framesCounter { 0 }, framesSpeed { framesSpeed }
{
    SDL_Point textureSize { tools::getSize(texture) };
    frameRec = { 0, 0, textureSize.x / 6, textureSize.y };
}

// TODO: Update position.
void AnimationSystem::update()
{
    ++framesCounter;

    if (framesCounter >= (60 / framesSpeed))
    {
        framesCounter = 0;
        ++currentFrame;

        if (currentFrame > 5)
            currentFrame = 0;

        frameRec.x = currentFrame * tools::getSize(texture).x / 6;
    }

    // Render part of the texture
    // TODO: Let user add color in constructor.
    window->renderRec(texture, &frameRec, position, WHITE);
}
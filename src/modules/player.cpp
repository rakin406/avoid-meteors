#include "modules/player.h"
#include "components.h"
#include "states.h"
#include "tags.h"

#include "SDL.h"
#include <flecs.h>

modules::Player::Player(flecs::world& world)
{
    world.module<Player>();

    world
        .system<Transform, const Velocity, tags::Player>("MovementSystem")
        .each(
            [&world](flecs::iter& it, size_t, Transform& transform,
                     const Velocity& vel, tags::Player)
            {
                SDL_PumpEvents();
                const Uint8* keyState { SDL_GetKeyboardState(nullptr) };

                // Continuous-response keys
                if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
                {
                    world.set(Direction::Left, Movement::Running);
                    transform.position.x -= vel.x * it.delta_time();
                }
                else if (keyState[SDL_SCANCODE_RIGHT] ||
                         keyState[SDL_SCANCODE_D])
                {
                    world.set(Direction::Right, Movement::Running);
                    transform.position.x += vel.x * it.delta_time();
                }
                else
                {
                    world.set(Movement::Idle);
                }
            });

    world.system<Animation, tags::Player>("AnimationSystem")
        .each(
            [&world](Animation& animation, tags::Player)
            {
                // Get the current value of the states
                const Movement* movement { world.get<Movement>() };
                // const Direction* direction { player.get<Direction>() };

                // Calculate the current frame based on time
                Uint32 currentTime { SDL_GetTicks() };
                // NOTE: This assumes you have 6 frames in each row.
                int currentFrame { (currentTime / animation.frameDuration) %
                                   6 };

                // Update the x-coordinate of the source rectangle
                animation.frameRec.x = animation.frameSize.x * currentFrame;

                switch (*movement)
                {
                case Movement::Idle:
                    animation.frameRec.y = 0;
                    break;
                case Movement::Running:
                    // TODO: Refactor this.
                    animation.frameRec.y = animation.frameSize.y;
                    break;
                default:
                    break;
                }
            });
}

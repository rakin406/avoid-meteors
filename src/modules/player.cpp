#include "modules/player.h"
#include "components.h"
#include "states.h"
#include "tags.h"

#include "SDL.h"
#include <flecs.h>

#include <iostream>

modules::Player::Player(flecs::world& world)
{
    world.module<Player>();
    world.component<Movement>().add(flecs::Union);
    world.component<Direction>().add(flecs::Union);

    // TODO: Change this.
    auto lastDirection { world.get_ref<Direction>() };

    world.system<Transform, const Velocity, tags::Player>("MovementSystem")
        .each(
            [&](Transform& transform, const Velocity& vel, tags::Player)
            {
                SDL_PumpEvents();
                const Uint8* keyState { SDL_GetKeyboardState(nullptr) };

                // Continuous-response keys
                if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
                {
                    world.set(Movement::Running);
                    world.set(Direction::Left);
                    transform.position.x -= vel.x * world.delta_time();
                }
                else if (keyState[SDL_SCANCODE_RIGHT] ||
                         keyState[SDL_SCANCODE_D])
                {
                    world.set(Movement::Running);
                    world.set(Direction::Right);
                    transform.position.x += vel.x * world.delta_time();
                }
                else
                {
                    world.set(Movement::Idle);
                }
            });

    world.system<Animation>("AnimationSystem")
        .each(
            [&](Animation& animation)
            {
                // Get the current value of the states
                const Movement* movement { world.get<Movement>() };
                const Direction* direction { world.get<Direction>() };

                // Calculate the current frame based on time
                Uint32 currentTime { SDL_GetTicks() };
                // NOTE: This assumes you have 6 frames in each row.
                int currentFrame { (currentTime / animation.frameDuration) %
                                   6 };

                // Update the x-coordinate of the source rectangle
                animation.frameRec.x = animation.frameRec.w * currentFrame;

                // TODO: Turn into a separate function.
                switch (*movement)
                {
                case Movement::Idle:
                    animation.frameRec.y = 0;
                    break;
                case Movement::Running:
                    // TODO: Refactor this.
                    animation.frameRec.y = animation.frameRec.h;
                    break;
                default:
                    break;
                }

                // if (direction == lastDirection.get())
                //{
                // }
                // else
                //{
                // }
                // lastDirection = *direction;
            });
}

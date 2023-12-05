#include "modules/player.h"
#include "components.h"
#include "constants.h"
#include "states.h"
#include "tags.h"
#include "tools.h"

#include "SDL.h"
#include <flecs.h>

#include <array>
#include <iostream>

namespace
{
    constexpr std::array<Direction, 2> ALL_DIRECTIONS { Direction::Left,
                                                        Direction::Right };
}

modules::Player::Player(flecs::world& world)
{
    using namespace constants;

    world.module<Player>();
    world.component<Movement>().add(flecs::Union);
    world.component<Direction>().add(flecs::Union);

    Direction randomDirection { ALL_DIRECTIONS[tools::getRandomValue(
        0, static_cast<int>(ALL_DIRECTIONS.size() - 1))] };

    // Set world states
    world.set(Movement::Idle);
    world.set(randomDirection);

    world.set<Animation>({ { 0, 0, static_cast<int>(player::FRAME_SIZE),
                             static_cast<int>(player::FRAME_SIZE) },
                           SDL_FLIP_NONE,
                           player::FRAME_DURATION });

    auto lastDirection { &randomDirection };

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
                Direction* direction { world.get_mut<Direction>() };

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
                    animation.frameRec.y = animation.frameRec.h;
                    break;
                default:
                    break;
                }

                // Add a flip based on direction
                if (direction == lastDirection)
                {
                    animation.flip = SDL_FLIP_NONE;
                }
                else
                {
                    animation.flip = SDL_FLIP_HORIZONTAL;
                }

                lastDirection = direction;
            });
}

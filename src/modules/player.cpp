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

    auto playerEntity { world.entity("Player") };
    playerEntity.add<tags::Player>()
        .add<tags::SpriteRenderer>()
        .add(Movement::Idle)
        .add(randomDirection)
        .set<Transform>({ player::STARTING_POSITION,
                          0.0f,
                          { player::FRAME_SCALE, player::FRAME_SCALE } })
        .set<Velocity>({ player::SPEED, 0.0f })
        .set<Animation>({ { 0, 0, static_cast<int>(player::FRAME_SIZE),
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
                    playerEntity.add(Movement::Running).add(Direction::Left);
                    transform.position.x -= vel.x * world.delta_time();
                }
                else if (keyState[SDL_SCANCODE_RIGHT] ||
                         keyState[SDL_SCANCODE_D])
                {
                    playerEntity.add(Movement::Running).add(Direction::Right);
                    transform.position.x += vel.x * world.delta_time();
                }
                else
                {
                    playerEntity.add(Movement::Idle);
                }
            });

    world.system<Animation>("AnimationSystem")
        .each(
            [&](Animation& animation)
            {
                // Get the current value of the states
                const Movement* movement { playerEntity.get<Movement>() };
                Direction* direction { playerEntity.get_mut<Direction>() };

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
                    std::cout << "same direction\n";
                    animation.flip = SDL_FLIP_NONE;
                }
                else
                {
                    std::cout << "different direction\n";
                    animation.flip = SDL_FLIP_HORIZONTAL;
                }

                lastDirection = direction;
            });
}

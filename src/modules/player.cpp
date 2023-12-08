#include "modules/player.h"
#include "components.h"
#include "tags.h"
#include "tools.h"

#include "SDL.h"
#include <flecs.h>

#include <iostream>

modules::Player::Player(flecs::world& world)
{
    // Setup player entity
    playerInit(world);

    // System that takes input and moves player entity
    world.system<Transform, const Velocity, PlayerTag>("Move")
        .kind(flecs::OnUpdate)
        .each(
            [](flecs::entity player, Transform& transform, const Velocity& vel,
               PlayerTag)
            {
                SDL_PumpEvents();
                const Uint8* keyState { SDL_GetKeyboardState(nullptr) };

                // Continuous-response keys
                if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
                {
                    player.add(Movement::Running);
                    player.add(Direction::Left);
                    transform.position.x -= vel.x * player.world().delta_time();
                }
                else if (keyState[SDL_SCANCODE_RIGHT] ||
                         keyState[SDL_SCANCODE_D])
                {
                    player.add(Movement::Running);
                    player.add(Direction::Right);
                    transform.position.x += vel.x * player.world().delta_time();
                }
                else
                {
                    player.add(Movement::Idle);
                }
            });

    // System that animates player entity
    world.system<Animation>("Animate")
        .kind(flecs::OnUpdate)
        .with<Movement>(flecs::Wildcard)
        .with<Direction>(flecs::Wildcard)
        .iter(
            [](flecs::iter& it, Animation* animation)
            {
                // Get the current value of the states
                auto movement { it.pair(2).second().to_constant<Movement>() };
                auto direction { it.pair(3).second().to_constant<Direction>() };

                // Calculate the current frame based on time
                Uint32 currentTime { SDL_GetTicks() };
                // NOTE: This assumes you have 6 frames in each row.
                int currentFrame { (currentTime / animation->frameDuration) %
                                   6 };

                // Update the x-coordinate of the source rectangle
                animation->frameRec.x = animation->frameRec.w * currentFrame;

                switch (movement)
                {
                case Movement::Idle:
                    animation->frameRec.y = 0;
                    break;
                case Movement::Running:
                    animation->frameRec.y = animation->frameRec.h;
                    break;
                default:
                    break;
                }

                // Add a flip based on direction
                switch (direction)
                {
                case Direction::Left:
                    animation->flip = SDL_FLIP_HORIZONTAL;
                    break;
                case Direction::Right:
                    animation->flip = SDL_FLIP_NONE;
                    break;
                default:
                    break;
                }
            });
}

void modules::Player::playerInit(flecs::world& world)
{
    auto player { world.entity("Player") };

    Direction randomDirection { ALL_DIRECTIONS[tools::getRandomValue(
        0, static_cast<int>(ALL_DIRECTIONS.size() - 1))] };

    // Set player components
    player.add<PlayerTag>()
        .add<Collider>()
        .add<SpriteRenderer>()
        .add(Movement::Idle)
        .add(randomDirection)
        .add<Sprite>()
        .set<Animation>({ { 0, 0, static_cast<int>(FRAME_SIZE),
                            static_cast<int>(FRAME_SIZE) },
                          SDL_FLIP_NONE,
                          FRAME_DURATION })
        .set<Transform>(
            { STARTING_POSITION, 0.0f, { FRAME_SCALE, FRAME_SCALE } })
        .set<Velocity>({ SPEED, 0.0f });
}

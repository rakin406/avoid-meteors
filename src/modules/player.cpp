#include "modules/player.h"
#include "components.h"
#include "constants.h"
#include "renderWindow.h"
#include "states.h"
#include "tags.h"
#include "tools.h"

#include "SDL.h"
#include <flecs.h>

#include <array>

namespace
{
    constexpr std::array<Direction, 2> ALL_DIRECTIONS { Direction::Left,
                                                        Direction::Right };
}

modules::Player::Player(flecs::world& world, RenderWindow& window)
{
    using namespace constants;

    world.module<Player>();

    // Set singleton
    world.add<tags::Player>();

    world
        .system<Transform, const Velocity, const tags::Player>("MovementSystem")
        .each(
            [this](flecs::iter& it, size_t, Transform& transform,
                   const Velocity& vel, tags::Player)
            {
                SDL_PumpEvents();
                const Uint8* keyState { SDL_GetKeyboardState(nullptr) };

                // Continuous-response keys
                if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
                {
                    player.add(Movement::Running).add(Direction::Left);
                    transform.position.x -= vel.x * it.delta_time();
                }
                else if (keyState[SDL_SCANCODE_RIGHT] ||
                         keyState[SDL_SCANCODE_D])
                {
                    player.add(Movement::Running).add(Direction::Right);
                    transform.position.x += vel.x * it.delta_time();
                }
                else
                {
                    player.add(Movement::Idle);
                }
            });

    world.system<Animation, const tags::Player>("AnimationSystem")
        .each(
            [this](Animation& animation, tags::Player)
            {
                // Get the current value of the states
                const Movement* movement { player.get<Movement>() };
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

    player = world.entity("Player");
    SDL_Texture* playerSprite { window.loadTexture(assets::PLAYER_SHEET) };
    Direction randomDirection { ALL_DIRECTIONS[tools::getRandomValue(
        0, static_cast<int>(ALL_DIRECTIONS.size() - 1))] };

    // Set player components
    player.add<tags::SpriteRenderer>()
        .add(Movement::Idle)
        .add(randomDirection)
        .set<Transform>({ player::STARTING_POSITION,
                          0.0f,
                          { player::FRAME_SCALE, player::FRAME_SCALE } })
        .set<Sprite>({ playerSprite, nullptr })
        // TODO: Set sdl_rect nullptr.
        .set<Animation>({ { 0, 0, static_cast<int>(player::FRAME_SIZE),
                            static_cast<int>(player::FRAME_SIZE) },
                          { player::FRAME_SIZE, player::FRAME_SIZE },
                          nullptr,
                          player::FRAME_DURATION })
        .set<Velocity>({ player::SPEED, 0.0f });
}

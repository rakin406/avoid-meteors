#include "game.h"
#include "colors.h"
#include "components.h"
#include "constants.h"
#include "renderWindow.h"
#include "states.h"
#include "tags.h"
#include "tools.h"

#include "SDL.h"
#include "SDL_image.h"
#include <flecs.h>

#include <array>
#include <iostream>

namespace
{
    constexpr std::array<Direction, 2> ALL_DIRECTIONS { Direction::Left,
                                                        Direction::Right };

    /**
     * @brief Returns true if user requests quit. For use in main loop.
     * @param event SDL_Event&
     * @return bool
     */
    bool isQuitRequested(const SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return true;
            break;
        default:
            break;
        }

        return false;
    }

    // TODO: Finish this.
    // void handleIdle(const Direction& direction, Animation& animation)
    //{
    //    using namespace constants;

    //    switch (direction)
    //    {
    //    // TODO: Add flip for direction.
    //    case Direction::Left:
    //        animation.currentClip =
    //            &animation.spriteClips[animation.currentFrame /
    //            PLAYER_FRAMES];
    //        break;
    //    case Direction::Right:
    //        animation.currentClip =
    //            &animation.spriteClips[animation.currentFrame /
    //            PLAYER_FRAMES];
    //        break;
    //    default:
    //        break;
    //    }
    //}

    // handleRunning();
} // namespace

Game::Game()
    : running { true },
      window { constants::window::WIDTH, constants::window::HEIGHT,
               constants::window::TITLE },
      background { window.loadTexture(constants::assets::BACKGROUND) }
{
}

void Game::run()
{
    init();
    while (running)
        update();
    stop();
}

void Game::init()
{
    using namespace constants;

    world
        .system<Transform, const Velocity, const Player>("PlayerMovementSystem")
        .each(
            [](flecs::iter& it, size_t index, Transform& transform,
               const Velocity& vel, Player)
            {
                SDL_PumpEvents();
                const Uint8* keyState { SDL_GetKeyboardState(nullptr) };

                auto player { it.entity(index) };

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

    world.system<Animation>("AnimationSystem")
        .each(
            [](flecs::entity entity, Animation& animation)
            {
                // Get the current value of the states
                const Movement* movement { entity.get<Movement>() };
                // const Direction* direction { entity.get<Direction>() };

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

    world
        .system<const Transform, const Sprite, const SpriteRenderer>(
            "SpriteRendererSystem")
        .each(
            [this](flecs::entity entity, const Transform& transform,
                   const Sprite& sprite, SpriteRenderer)
            {
                // Render player
                if (entity.has<Player>() && entity.has<Animation>())
                {
                    Animation* animation { entity.get_mut<Animation>() };
                    SDL_FRect dest { transform.position.x, transform.position.y,
                                     animation->frameSize.x * transform.scale.x,
                                     animation->frameSize.y *
                                         transform.scale.y };
                    window.render(sprite.texture, &animation->frameRec, &dest,
                                  transform.rotation, nullptr, sprite.color);
                }
            });

    auto player { world.entity("Player") };
    SDL_Texture* playerSprite { window.loadTexture(assets::PLAYER_SHEET) };
    Direction randomDirection { ALL_DIRECTIONS[tools::getRandomValue(
        0, static_cast<int>(ALL_DIRECTIONS.size() - 1))] };

    // Set player components
    player.add<Player>()
        .add<SpriteRenderer>()
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
                          player::FRAME_DURATION })
        .set<Velocity>({ player::SPEED, 0.0f });
}

void Game::update()
{
    // Get our controls and events
    while (SDL_PollEvent(&event))
    {
        if (isQuitRequested(event))
            running = false;
    }

    window.clear(WHITE);

    // Draw the background
    window.render(background, nullptr, nullptr, 0, nullptr);

    world.progress();

    window.display();
}

void Game::stop() { window.close(); }

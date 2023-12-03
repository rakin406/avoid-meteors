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

#include <iostream>
#include <vector>

namespace
{
    /**
     * @brief Split sprite sheet into a collection of rectangles.
     * @param frames Total number of frames in sheet.
     * @param rows Total rows in one column.
     * @param columns Total number of columns.
     * @param spriteSize Full sprite size.
     * @return std::vector<SDL_Rect>
     */
    std::vector<SDL_Rect> splitSpriteSheet(int frames, int rows, int columns,
                                           const SDL_Point& spriteSize)
    {
        const int FRAME_WIDTH { spriteSize.x / rows };
        const int FRAME_HEIGHT { spriteSize.y / columns };

        std::vector<SDL_Rect> clips(frames);
        SDL_Point currentPosition { 0, 0 };

        for (int i { 0 }; auto& frame : clips)
        {
            frame.x = currentPosition.x;
            frame.y = currentPosition.y;
            // TODO: Set rect size based on texture size ratio.
            frame.w = 64;
            frame.h = 205;

            if ((i + 1) % rows == 0)
            {
                currentPosition.x = 0;
                currentPosition.y += FRAME_HEIGHT; // Go to next column
            }
            else
            {
                currentPosition.x += FRAME_WIDTH; // Go to next row
            }

            ++i;
        }

        return clips;
    }

    /**
     * @brief Return true if user requests quit. For use in main loop.
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
    void handleIdle(const Direction& direction, SpriteAnimation& animation)
    {
        using namespace constants;

        switch (direction)
        {
        // TODO: Add flip for direction.
        case Direction::Left:
            animation.currentClip =
                &animation.spriteClips[animation.currentFrame / PLAYER_FRAMES];
            break;
        case Direction::Right:
            animation.currentClip =
                &animation.spriteClips[animation.currentFrame / PLAYER_FRAMES];
            break;
        default:
            // Cycle animation
            if ((animation.currentFrame / PLAYER_FRAMES) >= PLAYER_FRAMES)
            {
                animation.currentFrame = 0;
            }
            break;
        }
    }

    // handleRunning();
} // namespace

Game::Game()
    : running { true }, window { constants::WINDOW_WIDTH,
                                 constants::WINDOW_HEIGHT, "Avoid Meteors" },
      background { window.loadTexture(constants::BG_IMG_PATH) }
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

    world.component<Movement>().add(flecs::Union);
    world.component<Direction>().add(flecs::Union);

    world.system<Transform, const Velocity, const Player>("PlayerSystem")
        .each(
            [](flecs::entity entity, Transform& transform, const Velocity& vel,
               const Player& tag)
            {
                SDL_PumpEvents();
                const Uint8* keyState { SDL_GetKeyboardState(nullptr) };

                // Continuous-response keys
                if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
                {
                    entity.add(Movement::Running).add(Direction::Left);
                    transform.position.x -= vel.x;
                }
                else if (keyState[SDL_SCANCODE_RIGHT] ||
                         keyState[SDL_SCANCODE_D])
                {
                    entity.add(Movement::Running).add(Direction::Right);
                    transform.position.x += vel.x;
                }
                else
                {
                    entity.add(Movement::Idle);
                }
            });

    //world
    //    .system<const Transform, const Movement, const Direction, const Sprite,
    //            SpriteAnimation>("SpriteAnimationSystem")
    //    .each(
    //        [this](const Transform& transform, const Movement& movement,
    //               const Direction& direction, const Sprite& sprite,
    //               SpriteAnimation& animation)
    //        {
    //            switch (movement)
    //            {
    //            case Movement::Idle:
    //                handleIdle(direction, animation);
    //                break;
    //            case Movement::Running:
    //                // handleRunning();
    //                break;
    //            default:
    //                ++animation.currentFrame;
    //                window.render(sprite.texture, animation.currentClip,
    //                              transform.position);
    //                break;
    //            }
    //        });

    // world
    //     .system<const Transform, const Sprite, const SpriteRenderer>(
    //         "SpriteRendererSystem")
    //     .each(
    //         [this](flecs::entity entity, const Transform& transform,
    //                const Sprite& sprite, const SpriteRenderer&
    //                spriteRenderer)
    //         {
    //             const SDL_Color* tint { entity.get<SDL_Color>() };
    //             // window.render(sprite.texture);
    //         });

    auto player { world.entity("Player") };
    SDL_Texture* playerSprite { window.loadTexture(
        constants::PLAYER_SHEET_PATH) };
    Direction randomDirection {
        ALL_DIRECTIONS[tools::getRandomValue(0, ALL_DIRECTIONS.size() - 1)]
    };

    player.add<Player>()
        .add<SpriteRenderer>()
        .add(Movement::Idle)
        .add(randomDirection)
        .set<Transform>({ { 200, 200 }, { 0, nullptr }, { 1, 1 } })
        .set<Sprite>({ playerSprite })
        .set<SpriteAnimation>(
            { PLAYER_FRAMES, 0, nullptr,
              splitSpriteSheet(PLAYER_FRAMES, 6, 2,
                               tools::getSize(playerSprite)) })
        .set<Velocity>({ PLAYER_SPEED, PLAYER_SPEED });
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
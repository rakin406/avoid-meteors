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
     * @brief Splits sprite sheet into a collection of rectangles.
     * @param frames Total number of frames in sheet.
     * @param rows Total rows in one column.
     * @param columns Total number of columns.
     * @param spriteSize Full sprite size.
     * @return A vector of SDL rectangles.
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

    world.system<Animation>("AnimationSystem")
        .each(
            [](flecs::entity entity, Animation& animation)
            {
                // Get the current value of the states
                const Movement* movement { entity.get<Movement>() };
                // const Direction* direction { entity.get<Direction>() };

                // Calculate the current frame based on time
                Uint32 currentTime { SDL_GetTicks() };
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
                   const Sprite& sprite, const SpriteRenderer& spriteRenderer)
            {
                // Render player
                if (entity.has<Player>() && entity.has<Animation>())
                {
                    Animation* animation { entity.get_mut<Animation>() };
                    SDL_Rect dest { transform.position.x, transform.position.y,
                                    animation->frameSize.x * transform.scale.x,
                                    animation->frameSize.y *
                                        transform.scale.y };
                    window.render(sprite.texture, &animation->frameRec, &dest,
                                  transform.rotation, nullptr, sprite.color);
                }
            });

    auto player { world.entity("Player") };
    SDL_Texture* playerSprite { window.loadTexture(
        constants::PLAYER_SHEET_PATH) }; // Load player sprite sheet
    Direction randomDirection {
        ALL_DIRECTIONS[tools::getRandomValue(0, ALL_DIRECTIONS.size() - 1)]
    };

    // Set player components
    player.add<Player>()
        .add<SpriteRenderer>()
        .add(Movement::Idle)
        .add(randomDirection)
        .set<Transform>({ { WINDOW_WIDTH / 2, 200 }, 0, PLAYER_SCALE })
        .set<Sprite>({ playerSprite, nullptr })
        .set<Animation>({ { 0, 0, 32, 32 }, { 32, 32 }, 250 })
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
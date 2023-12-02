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
    // TODO: Finish this function.
    std::vector<SDL_Rect> splitSpriteSheet(int frames, const SDL_Point& size)
    {
        std::vector<SDL_Rect> clips {};
        SDL_Point currentPosition {};

        for (int frameIndex { 0 }; frameIndex < frames; ++frameIndex)
        {
            // NOTE: This function assumes that there is 6 frames in each rows.
            if ((frameIndex + 1) % 6 == 0)
            {
            }
            clips[frameIndex].x = currentPosition.x;
            clips[frameIndex].y = currentPosition.y;
            // TODO: Set rect size based on texture size ratio.
            clips[frameIndex].w = 64;
            clips[frameIndex].h = 205;
        }
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
    void handleIdle(const Direction& direction)
    {
        switch (direction)
        {
        case Direction::Left:
            break;
        case Direction::Right:
            break;
        default:
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

    world
        .system<const Movement, const Direction, const Sprite, SpriteAnimation>(
            "SpriteAnimationSystem")
        .each(
            [](const Movement& movement, const Direction& direction,
               const Sprite& sprite, SpriteAnimation& animation)
            {
                switch (movement)
                {
                case Movement::Idle:
                    // handleIdle();
                    break;
                case Movement::Running:
                    // handleRunning();
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
                const SDL_Color* tint { entity.get<SDL_Color>() };
                window.renderEx(sprite.texture, transform.position,
                                transform.rotation.angle,
                                transform.rotation.center, transform.scale,
                                tint);
            });

    flecs::entity player { world.entity("Player") };
    Direction randomDirection {
        ALL_DIRECTIONS[tools::getRandomValue(0, ALL_DIRECTIONS.size() - 1)]
    };
    player.add<Player>()
        .add<SpriteRenderer>()
        .add(Movement::Idle)
        .add(randomDirection)
        .set<Transform>({ { 200, 200 }, { 0, nullptr }, { 1, 1 } })
        .set<Sprite>({ window.loadTexture(constants::PLAYER_SHEET_PATH) })
        // TODO: Finish this
        .set<SpriteAnimation>({
            PLAYER_FRAMES,
        })
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
    window.renderPro(background, nullptr, nullptr, 0, nullptr);

    world.progress();

    window.display();
}

void Game::stop() { window.close(); }
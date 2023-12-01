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

namespace
{
    /**
     * @brief Initialize SDL systems.
     */
    void initSDL()
    {
        // Initialize SDL's systems and check for errors
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: "
                      << SDL_GetError() << std::endl;
        }

        // Initialize SDL image
        if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)))
        {
            std::cout << "IMG_init has failed. Error: " << SDL_GetError()
                      << std::endl;
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
} // namespace

Game::Game()
    : running { true }, window { constants::WINDOW_WIDTH,
                                 constants::WINDOW_HEIGHT, "Avoid Meteors" },
      background { window.loadTexture(constants::BG_IMG_PATH) }
{
    initSDL();
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

    flecs::system playerSystem {
        world.system<Transform, const Velocity, const Player>().each(
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
            })
    };

    // TODO: Finish system.
    flecs::system spriteRendererSystem {
        world.system<const Transform, const Sprite, const SpriteRenderer>()
            .each(
                [this](flecs::entity entity, const Transform& transform,
                       const Sprite& sprite,
                       const SpriteRenderer& spriteRenderer)
                {
                    const SDL_Color* tint { entity.get<SDL_Color>() };
                    if (tint)
                    {
                        // TODO: Clean this mess...
                        window.renderEx(sprite.texture, *transform.position, 
                            transform.rotation.angle, transform.rotation.center, 
                            *transform.scale, tint);
                    }
                    else
                    {
                        window.renderEx(sprite.texture, *transform.position, 
                            transform.rotation.angle, transform.rotation.center, 
                            *transform.scale);
                    }
                })
    };

    flecs::entity player { world.entity("Player") };
    Direction randomDirection {
        ALL_DIRECTIONS[tools::getRandomValue(0, ALL_DIRECTIONS.size() - 1)]
    };
    player.add<Player>()
        .add<SpriteRenderer>()
        .add<SpriteAnimation>()
        .add(Movement::Idle)
        .add(randomDirection)
        // FIX:
        .set<Transform>({ { 200, 200 }, {}, { 1, 1 } })
        .set<Sprite>({ window.loadTexture(constants::PLAYER_SHEET_PATH) })
        .set<Velocity>({ PLAYER_SPEED, PLAYER_SPEED });

    playerSystem.run();
}

void Game::update()
{
    // Get our controls and events
    while (SDL_PollEvent(&event))
    {
        if (isQuitRequested(event))
            running = false;
    }

    world.progress();

    window.clear(WHITE);

    // Draw the background
    window.renderPro(background, nullptr, nullptr, 0, nullptr);

    window.display();
}

void Game::stop() { window.close(); }
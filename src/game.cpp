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

    /**
     * @brief Create all the necessary states for the world.
     * @param world
     */
    void createStates(flecs::world& world)
    {
        world.component<Movement>().add(flecs::Union);
        world.component<Direction>().add(flecs::Union);

        // Create a few entities with various state combinations
        world.entity("IdleLeft").add(Movement::Idle).add(Direction::Left);
        world.entity("IdleRight").add(Movement::Idle).add(Direction::Right);
        world.entity("RunningLeft").add(Movement::Running).add(Direction::Left);
        world.entity("RunningRight")
            .add(Movement::Running)
            .add(Direction::Right);
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

    flecs::world world {};

    createStates(world);

    playerSystem = world.system<const Player, Position>().each(
        [](const Player& tag, Position& pos)
        {
            SDL_PumpEvents();
            const Uint8* keyState { SDL_GetKeyboardState(nullptr) };

            // Continuous-response keys
            if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
            {
                std::cout << "it works!\n";
                // TODO: Change position.
            }
            else if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
            {
            }
        });

    flecs::entity player { world.entity("Player") };
    Direction randomDirection {
        ALL_DIRECTIONS[tools::getRandomValue(0, ALL_DIRECTIONS.size() - 1)]
    };
    // TODO: Add velocity component to player.
    player.add<Player>().add(Movement::Idle).add(randomDirection);
    player.set<IdleTexture>({ window.loadTexture(IDLE_SPRITE) })
        .set<RunningTexture>({ window.loadTexture(RUNNING_SPRITE) })
        .set<SDL_Color>(WHITE)
        // TODO: Start position should be center and on ground.
        .set<Position>({ 500, 500 });
}

void Game::update()
{
    // Get our controls and events
    while (SDL_PollEvent(&event))
    {
        if (isQuitRequested(event))
            running = false;
    }

    playerSystem.run();

    window.clear(WHITE);

    // Draw the background
    window.renderPro(background, nullptr, nullptr, 0, nullptr, WHITE);

    window.display();
}

void Game::stop() { window.close(); }
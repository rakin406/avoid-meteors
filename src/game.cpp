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
        if (event.type == SDL_QUIT)
        {
            return true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return true;
        }

        return false;
    }
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

    initSDL();

    flecs::world world {};

    flecs::entity player { world.entity() };
    Direction randomDirection {
        ALL_DIRECTIONS[tools::getRandomValue(0, ALL_DIRECTIONS.size() - 1)]
    };
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

    window.clear(WHITE);

    // Draw the background
    window.renderPro(background, nullptr, nullptr, 0, nullptr, WHITE);

    window.display();
}

void Game::stop() { window.close(); }
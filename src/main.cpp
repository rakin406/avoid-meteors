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
#include <string_view>

namespace
{
    /**
     * @brief Initialize all SDL systems.
     * @return true if success.
     */
    bool init()
    {
        // Initialize SDL's systems and check for errors
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: "
                      << SDL_GetError() << std::endl;
            return false;
        }

        // Initialize SDL image
        if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)))
        {
            std::cout << "IMG_init has failed. Error: " << SDL_GetError()
                      << std::endl;
            return false;
        }

        return true;
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

int main(int argv, char** args)
{
    using namespace constants;

    if (!(init()))
        return -1;

    RenderWindow window { WINDOW_WIDTH, WINDOW_HEIGHT, "Avoid Meteors" };
    SDL_Texture* bgTexture { window.loadTexture(BG_IMG_PATH) };

    flecs::world world {};

    flecs::entity player { world.entity() };
    player.add<Player>()
        .add(Movement::Idle)
        .add(Direction::Right); // TODO: Make start direction random
    player.set<IdleTexture>({ window.loadTexture(IDLE_SPRITE) })
        .set<RunningTexture>({ window.loadTexture(RUNNING_SPRITE) })
        .set<Color>(WHITE)
        // TODO: Start position should be center and on ground.
        .set<Position>({ 500, 500 });

    bool gameRunning { true };
    SDL_Event event {};

    // Main game loop
    while (gameRunning)
    {
        // Get our controls and events
        while (SDL_PollEvent(&event))
        {
            if (isQuitRequested(event))
                gameRunning = false;
        }

        window.clear(WHITE);

        // Draw the background
        window.renderPro(bgTexture, nullptr, nullptr, 0, nullptr, WHITE);

        window.display();
    }

    // De-Initialization
    window.close();

    return 0;
}
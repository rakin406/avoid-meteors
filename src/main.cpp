#include "colors.h"
#include "components.h"
#include "constants.h"
#include "renderWindow.h"
#include "tags.h"

#include "SDL.h"
#include "SDL_image.h"
#include <flecs.h>

#include <iostream>
#include <string_view>

namespace
{
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

    // Initialize SDL's systems and check for errors
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError()
                  << std::endl;
        return -1;
    }

    // Initialize SDL image
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)))
    {
        std::cout << "IMG_init has failed. Error: " << SDL_GetError()
                  << std::endl;
        return -2;
    }

    RenderWindow window { WINDOW_WIDTH, WINDOW_HEIGHT, "Avoid Meteors" };
    SDL_Texture* bgTexture { window.loadTexture(BG_IMG_PATH) };

    flecs::world world;

    auto player { world.entity() };
    player.add<Player>();
    // NOTE: This is only for idle sprite.
    // FIX: SDL_Texture can't be added to flecs?
    //player.set<SDL_Texture*>(window.loadTexture(IDLE_SPRITE))
    //    .set<Position>({ 500, 500 });

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

        // TODO: Change this.
        // Draw the background
        window.render(bgTexture, nullptr, nullptr);

        window.display();
    }

    // De-Initialization
    window.close();

    return 0;
}
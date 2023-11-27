#include "components.h"
#include "renderWindow.h"
#include "tags.h"

#include "SDL.h"
#include "SDL_image.h"
#include <flecs.h>

namespace
{
    /**
     * @brief Return true if user requests quit. For use in main loop.
     *
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

int main()
{
    // flecs::world world;

    // auto player = world.entity();
    // player.add<Player>();

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    RenderWindow window { "Avoid Meteors", 1000, 746 };
    SDL_Texture* bgTexture = window.loadTexture("res/gfx/background.jpg");

    bool gameRunning { true };
    SDL_Event event {};

    while (gameRunning)
    {
        // Get our controls and events
        while (SDL_PollEvent(&event))
        {
            if (isQuitRequested(event))
                gameRunning = false;
        }

        window.display();
    }

    window.destroy();

    return 0;
}
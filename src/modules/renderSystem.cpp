#include "modules/renderSystem.h"
#include "components.h"
#include "renderWindow.h"
#include "tags.h"

#include "SDL.h"

namespace
{
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

} // namespace

modules::RenderSystem::RenderSystem(flecs::world& world) {}

void modules::RenderSystem::windowInit(flecs::world& world)
{
    // Set singletons
    world.emplace<RenderWindow>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    world.add<SDL_Event>();

    // Set background components
    auto background { world.entity("Background") };
    background.add<Background>()
        .add<SpriteRenderer>()
        .add<Sprite>()
        .add<Transform>();
}

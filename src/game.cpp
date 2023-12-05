#include "game.h"
#include "colors.h"
#include "components.h"
#include "constants.h"
#include "renderWindow.h"
#include "tags.h"

#include "SDL.h"
#include "SDL_image.h"
#include <flecs.h>

#include <iostream>

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

Game::Game()
    : running { true },
      window { constants::window::WIDTH, constants::window::HEIGHT,
               constants::window::TITLE },
      background { window.loadTexture(constants::assets::BACKGROUND) }
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

    world
        .system<const Transform, const Sprite, const tags::SpriteRenderer>(
            "SpriteRendererSystem")
        .each(
            [this](flecs::entity entity, const Transform& transform,
                   const Sprite& sprite, tags::SpriteRenderer)
            {
                // Render player
                if (entity.has<tags::Player>() && entity.has<Animation>())
                {
                    Animation* animation { entity.get_mut<Animation>() };
                    SDL_FRect dest { transform.position.x, transform.position.y,
                                     animation->frameSize.x * transform.scale.x,
                                     animation->frameSize.y *
                                         transform.scale.y };
                    window.render(sprite.texture, &animation->frameRec, &dest,
                                  transform.rotation, nullptr, sprite.color,
                                  animation->flip);
                }
            });
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

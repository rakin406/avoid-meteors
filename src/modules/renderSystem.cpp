#include "modules/renderSystem.h"
#include "colors.h"
#include "components.h"
#include "constants.h"
#include "renderWindow.h"
#include "tags.h"

#include "SDL.h"

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

modules::RenderSystem::RenderSystem(flecs::world& world)
{
    world.module<RenderSystem>();

    // Register components
    world.component<Background>();
    world.component<SpriteRenderer>();
    world.component<Sprite>();
    world.component<Transform>();
    world.component<Animation>();
    world.component<Player>();

    // Setup window
    windowInit(world);

    // System that loads background texture on startup
    world.system<RenderWindow, Sprite, Background>("LoadBackground")
        .kind(flecs::OnStart)
        .term_at(1)
        .singleton()
        .each([](RenderWindow& window, Sprite& sprite, Background)
              { sprite.texture = window.loadTexture(BACKGROUND); });

    // System that loads font on startup
    world.system<RenderWindow>("LoadFont")
        .kind(flecs::OnStart)
        .term_at(1)
        .singleton()
        .each([](RenderWindow& window)
              { window.loadFont(DEFAULT_FONT, DEFAULT_FONT_SIZE); });

    auto backgroundRenderer {
        world
            .system<const Sprite, RenderWindow, SpriteRenderer, Background>(
                "RenderBackground")
            .kind(0)
            .term_at(2)
            .singleton()
            .each(
                [](const Sprite& sprite, RenderWindow& window, SpriteRenderer,
                   Background)
                {
                    window.render(sprite.texture, nullptr, nullptr, 0, nullptr,
                                  SDL_FLIP_NONE, sprite.color);
                })
    };

    auto textRenderer { world.system<RenderWindow, const Text>("RenderText")
                            .kind(0)
                            .term_at(1)
                            .singleton()
                            .term_at(2)
                            .singleton()
                            .each(
                                [](RenderWindow& window, const Text& text)
                                {
                                    window.setFontSize(text.size);
                                    window.render(text.texture, text.position);
                                }) };

    auto playerRenderer {
        world
            .system<const Transform, const Sprite, RenderWindow, Animation,
                    SpriteRenderer, Player>("RenderPlayer")
            .kind(0)
            .term_at(3)
            .singleton()
            .each(
                [](const Transform& transform, const Sprite& sprite,
                   RenderWindow& window, Animation& animation, SpriteRenderer,
                   Player)
                {
                    SDL_FRect dest { transform.position.x, transform.position.y,
                                     animation.frameRec.w * transform.scale.x,
                                     animation.frameRec.h * transform.scale.y };
                    window.render(sprite.texture, &animation.frameRec, &dest,
                                  transform.rotation, nullptr, animation.flip,
                                  sprite.color);
                })
    };

    // System that updates and renders game
    world.system<SDL_Event, RenderWindow>("UpdateWindow")
        .kind(flecs::PostUpdate)
        .term_at(1)
        .singleton()
        .term_at(2)
        .singleton()
        .each(
            [=, &world](SDL_Event& event, RenderWindow& window)
            {
                // Get our controls and events
                while (SDL_PollEvent(&event))
                {
                    if (isQuitRequested(event))
                        world.quit();
                }

                window.clear(WHITE);

                // Render entities
                backgroundRenderer.run();
                textRenderer.run();
                playerRenderer.run();

                window.display();
            });
}

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

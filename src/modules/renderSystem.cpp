#include "modules/renderSystem.h"
#include "components.h"
#include "constants.h"
#include "graphics/colors.h"
#include "graphics/renderWindow.h"
#include "tags.h"
#include "tools.h"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <iostream>

namespace {
/**
 * @brief Returns true if user requests quit. For use in main loop.
 * @param event SDL_Event&
 * @return bool
 */
bool isQuitRequested(const SDL_Event& event) {
    switch (event.type) {
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

}  // namespace

modules::RenderSystem::RenderSystem(flecs::world& world) {
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
        .each(
            [](RenderWindow& window, Sprite& sprite, Background) { sprite.texture = window.loadTexture(BACKGROUND); });

    // System that loads font on startup
    world.system<RenderWindow>("LoadFont").kind(flecs::OnStart).term_at(1).singleton().each([](RenderWindow& window) {
        window.loadFont(DEFAULT_FONT, DEFAULT_FONT_SIZE);
    });

    auto backgroundRenderer{world.system<const Sprite, RenderWindow, SpriteRenderer, Background>("RenderBackground")
                                .kind(0)
                                .term_at(2)
                                .singleton()
                                .each([](const Sprite& sprite, RenderWindow& window, SpriteRenderer, Background) {
                                    window.render(sprite.texture, nullptr, nullptr, 0, nullptr, SDL_FLIP_NONE,
                                                  sprite.color);
                                })};

    auto meteorsRenderer{
        world.system<const Transform, const Sprite, RenderWindow, SpriteRenderer, Meteor>("RenderMeteors")
            .kind(0)
            .term_at(3)
            .singleton()
            .each([](const Transform& transform, const Sprite& sprite, RenderWindow& window, SpriteRenderer, Meteor) {
                // TODO: Since texture size is constant, maybe make
                // it part of a component to improve performance?
                glm::vec2 textureSize{tools::getSize(sprite.texture)};
                SDL_FRect dest{transform.position.x, transform.position.y, textureSize.x * transform.scale.x,
                               textureSize.y * transform.scale.y};
                window.render(sprite.texture, nullptr, &dest, transform.rotation, nullptr, SDL_FLIP_NONE, sprite.color);
            })};

    auto textRenderer{
        world.system<RenderWindow, const Text>("RenderText")
            .kind(0)
            .term_at(1)
            .singleton()
            .term_at(2)
            .singleton()
            .each([](RenderWindow& window, const Text& text) { window.render(text.texture, text.position); })};

    auto playerRenderer{
        world.system<const Transform, const Sprite, RenderWindow, Animation, SpriteRenderer, Player>("RenderPlayer")
            .kind(0)
            .term_at(3)
            .singleton()
            .each([](const Transform& transform, const Sprite& sprite, RenderWindow& window, Animation& animation,
                     SpriteRenderer, Player) {
                SDL_FRect dest{transform.position.x, transform.position.y, sprite.textureRect.w * transform.scale.x,
                               sprite.textureRect.h * transform.scale.y};
                window.render(sprite.texture, &sprite.textureRect, &dest, transform.rotation, nullptr, animation.flip,
                              sprite.color);
            })};

    // System that updates and renders game
    world.system<SDL_Event, RenderWindow>("UpdateWindow")
        .kind(flecs::PostUpdate)
        .term_at(1)
        .singleton()
        .term_at(2)
        .singleton()
        .each([=, &world](SDL_Event& event, RenderWindow& window) {
            // Get our controls and events
            while (SDL_PollEvent(&event)) {
                if (isQuitRequested(event))
                    world.quit();
            }

            window.clear(WHITE);

            // Render entities
            backgroundRenderer.run();
            // meteorsRenderer.run();
            textRenderer.run();
            playerRenderer.run();

            window.display();
        });
}

void modules::RenderSystem::windowInit(flecs::world& world) {
    // Set singletons
    world.emplace<RenderWindow>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    world.add<SDL_Event>();

    // Set background components
    auto background{world.entity("Background")};
    background.add<Background>().add<SpriteRenderer>().add<Sprite>().add<Transform>();
}

#include "game.h"
#include "collisionLayer.h"
#include "colors.h"
#include "components.h"
#include "constants.h"
#include "modules/player.h"
#include "renderWindow.h"
#include "tags.h"
#include "tools.h"

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

void Game::run()
{
    init();
    while (world.progress())
    {
        // Run systems
    }
}

void Game::init()
{
    using namespace constants;

    world.import <modules::Player>();

    // Set singletons
    world.emplace<RenderWindow>(window::WIDTH, window::HEIGHT, window::TITLE);
    world.add<SDL_Event>();

    // Load assets on startup
    world.system<RenderWindow, Sprite>("LoadAssets")
        .kind(flecs::OnStart)
        .term_at(1)
        .singleton()
        .each(
            [](flecs::entity entity, RenderWindow& window, Sprite& sprite)
            {
                SDL_Texture* texture { nullptr };

                // Load background texture
                if (entity.has<Background>())
                {
                    texture = window.loadTexture(assets::BACKGROUND);
                }
                // Load player sprite sheet
                else if (entity.has<Player>())
                {
                    texture = window.loadTexture(assets::PLAYER_SHEET);
                }

                sprite.texture = texture;
            });

    // Check for collisions between entities
    world.system<Collider>("CollisionSystem")
        .kind(flecs::PostUpdate)
        .each(
            [](flecs::entity entity, Collider)
            {
                if (entity.has<Player>())
                {
                    const Transform* transform { entity.get<Transform>() };
                    // NOTE: I have no idea how the hell this is working...
                    if (transform->position.x <= -player::FRAME_SIZE ||
                        transform->position.x >=
                            (window::WIDTH -
                             (player::FRAME_SIZE * (player::FRAME_SCALE - 1))))
                    {
                        entity.add<CollidesWith, CollisionLayer::Wall>();
                    }
                }
            });

    auto spriteRendererSystem {
        world
            .system<const Transform, const Sprite, RenderWindow,
                    SpriteRenderer>("SpriteRendererSystem")
            .kind(0)
            .term_at(3)
            .singleton()
            .each(
                [](flecs::entity entity, const Transform& transform,
                   const Sprite& sprite, RenderWindow& window, SpriteRenderer)
                {
                    // Render background
                    if (entity.has<Background>())
                    {
                        window.render(sprite.texture, nullptr, nullptr, 0,
                                      nullptr, SDL_FLIP_NONE, sprite.color);
                    }
                    // Render player
                    else if (entity.has<Player>() && entity.has<Animation>())
                    {
                        Animation* animation { entity.get_mut<Animation>() };
                        SDL_FRect dest {
                            transform.position.x, transform.position.y,
                            animation->frameRec.w * transform.scale.x,
                            animation->frameRec.h * transform.scale.y
                        };
                        window.render(sprite.texture, &animation->frameRec,
                                      &dest, transform.rotation, nullptr,
                                      animation->flip, sprite.color);
                    }
                })
    };

    // Update and render game
    world.system<SDL_Event, RenderWindow>("UpdateWindow")
        .kind(flecs::PostUpdate)
        .term_at(1)
        .singleton()
        .term_at(2)
        .singleton()
        .each(
            [=](SDL_Event& event, RenderWindow& window)
            {
                // Get our controls and events
                while (SDL_PollEvent(&event))
                {
                    if (isQuitRequested(event))
                        world.quit();
                }

                window.clear(WHITE);

                spriteRendererSystem.run();

                window.display();
            });

    // Set background components
    auto background { world.entity<Background>() };
    background.add<Background>()
        .add<SpriteRenderer>()
        .add<Sprite>()
        .add<Transform>();
}

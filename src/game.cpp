#include "game.h"
#include "collisionLayer.h"
#include "colors.h"
#include "components.h"
#include "constants.h"
#include "modules/player.h"
#include "renderWindow.h"
#include "tags.h"

#include "SDL.h"
#include <flecs.h>

#include <iostream>

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
    using namespace modules;

    world.import <Player>();

    // System that loads background texture on startup
    world.system<RenderWindow, Sprite, Background>("LoadBackground")
        .kind(flecs::OnStart)
        .term_at(1)
        .singleton()
        .each([](RenderWindow& window, Sprite& sprite, Background)
              { sprite.texture = window.loadTexture(assets::BACKGROUND); });

    // System that checks for collisions between entities
    world.system<Collider>("Collision")
        .kind(flecs::PostUpdate)
        .each(
            [](flecs::entity entity, Collider)
            {
                if (entity.has<Player::PlayerTag>())
                {
                    const Transform* transform { entity.get<Transform>() };
                    // NOTE: I have no idea how the hell this is working...
                    if (transform->position.x <= -Player::FRAME_SIZE ||
                        transform->position.x >=
                            (window::WIDTH -
                             (Player::FRAME_SIZE * (Player::FRAME_SCALE - 1))))
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
                    else if (entity.has<Player::PlayerTag>() &&
                             entity.has<Player::Animation>())
                    {
                        Player::Animation* animation {
                            entity.get_mut<Player::Animation>()
                        };
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

    // System that updates and renders game
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
}

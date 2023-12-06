#include "game.h"
#include "colors.h"
#include "components.h"
#include "constants.h"
#include "modules/player.h"
#include "renderWindow.h"
#include "states.h"
#include "tags.h"
#include "tools.h"

#include "SDL.h"
#include "SDL_image.h"
#include <flecs.h>

#include <array>
#include <iostream>

namespace
{
    constexpr std::array<Direction, 2> ALL_DIRECTIONS { Direction::Left,
                                                        Direction::Right };

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

    world.set_target_fps(1);

    // Set window as a singleton
    world.set<RenderWindow>({ window::WIDTH, window::HEIGHT, window::TITLE });

    // Load assets on startup
    world.system<RenderWindow, Sprite>("LoadAssets")
        .kind(flecs::OnStart)
        .term_at(1)
        .singleton()
        .each(
            [this](RenderWindow& window, Sprite& sprite)
            {
                // Get entities
                auto background { world.entity<tags::Background>() };
                auto player { world.entity<tags::Player>() };

                // Load background texture
                if (background.is_valid())
                {
                    SDL_Texture* bgTexture { window.loadTexture(
                        assets::BACKGROUND) };
                    background.set<Sprite>({ bgTexture, nullptr });
                }

                // Load player sprite sheet
                if (player.is_valid())
                {
                    SDL_Texture* texture { window.loadTexture(
                        assets::PLAYER_SHEET) };
                    player.set<Sprite>({ texture, nullptr });
                }
            });

    world
        .system<const Transform, const Sprite, RenderWindow,
                tags::SpriteRenderer>("SpriteRendererSystem")
        .kind(flecs::PostUpdate)
        .term_at(3)
        .singleton()
        .each(
            [this](flecs::entity entity, const Transform& transform,
                   const Sprite& sprite, RenderWindow& window,
                   tags::SpriteRenderer)
            {
                // Render background
                if (entity.has<tags::Background>())
                {
                    window.render(sprite.texture, nullptr, nullptr, 0, nullptr,
                                  SDL_FLIP_NONE, sprite.color);
                }
                // Render player
                else if (entity.has<tags::Player>() && entity.has<Animation>())
                {
                    Animation* animation { entity.get_mut<Animation>() };
                    SDL_FRect dest { transform.position.x, transform.position.y,
                                     animation->frameRec.w * transform.scale.x,
                                     animation->frameRec.h *
                                         transform.scale.y };
                    window.render(sprite.texture, &animation->frameRec, &dest,
                                  transform.rotation, nullptr, animation->flip,
                                  sprite.color);
                }
            });

    SDL_Event event {};

    // Update and render game
    world.system<RenderWindow>("UpdateWindow")
        .kind(flecs::PostUpdate)
        .term_at(1)
        .singleton()
        .each(
            [&](RenderWindow& window)
            {
                // Get our controls and events
                while (SDL_PollEvent(&event))
                {
                    if (isQuitRequested(event))
                        world.quit();
                }

                window.clear(WHITE);
                window.display();
            });

    // Set background components
    auto background { world.entity<tags::Background>() };
    background.add<tags::Background>()
        .add<tags::SpriteRenderer>()
        .add<Transform>();

    auto player { world.entity<tags::Player>() };

    Direction randomDirection { ALL_DIRECTIONS[tools::getRandomValue(
        0, static_cast<int>(ALL_DIRECTIONS.size() - 1))] };

    // Set player components
    player.add<tags::Player>()
        .add<tags::SpriteRenderer>()
        .add(Movement::Idle)
        .add(randomDirection)
        .set<Transform>({ player::STARTING_POSITION,
                          0.0f,
                          { player::FRAME_SCALE, player::FRAME_SCALE } })
        .set<Velocity>({ player::SPEED, 0.0f })
        .set<Animation>({ { 0, 0, static_cast<int>(player::FRAME_SIZE),
                            static_cast<int>(player::FRAME_SIZE) },
                          SDL_FLIP_NONE,
                          player::FRAME_DURATION });
}
